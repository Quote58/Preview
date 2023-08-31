#include "file.h"

File::File(wxString path) {
	_file = new wxFile(path, wxFile::read_write);

	if (_file->IsOpened()) {
		_dataBuffer = (wxByte *) malloc(_file->Length());
		_file->Read(_dataBuffer, _file->Length());

	} else {
		wxLogError("File could not be opened!");
	}

	_filePath = path;
	wxFileName name(path);
	_name = name.GetName();
	_size = _file->Length();
}

void File::saveToFile() {
	_file->Seek(0);
	_file->Write(_dataBuffer, _file->Length());
	std::cout << "data buffer written over the file" << std::endl;
}

wxByte File::getByte(long offset) {
	if (offset < _size) {
		return _dataBuffer[offset];

	} else {
		std::cout << "invalid offset! Can't access offset, returning 0xFF instead " << offset << std::endl;
		return 0xFF;
	}
}

void File::setByte(long offset, wxByte byte) {
	if (offset >= _size) {
		std::cout << "invalid offset! Can't access offset " << offset << std::endl;
		return;
	}

	_dataBuffer[offset] = byte;
}

/* Palette file functions */
PaletteFile::PaletteFile(wxString path) : File(path) {
	// If there is a TPL signature, we know it's a .tpl file
	if ((_dataBuffer[kTPLSig] == 'T') && (_dataBuffer[kTPLSig + 1] == 'P') && (_dataBuffer[kTPLSig + 2] == 'L')) {
		_palType = kPalTPL;
		if (_dataBuffer[kTPLFormat] == kTPLRGB) {
			_bitdepth = 8;
			_palTPLFormat = kTPLRGB;
		
		} else if (_dataBuffer[kTPLFormat] == kTPLNES) {
			_bitdepth = 8;
			_palTPLFormat = kTPLNES;
		
		} else {
			_bitdepth = 5;
			_palTPLFormat = kTPLOther;
		}

	// Else it's either .pal or .act, for now we assume .pal
	} else {
		_palType = kPalPAL;

		// The start of a PSP .pal file is JASC-PAL
		if ((_dataBuffer[kTPLSig] == 'J') && (_dataBuffer[kTPLSig + 1] == 'A') && (_dataBuffer[kTPLSig + 2] == 'S') && (_dataBuffer[kTPLSig + 2] == 'C')) {
			_palPALFormat = kPalPSP;
		}
		
		// If it's a .pal but not a PSP, we are going to assume it is raw (it could also be microsoft, but for now we won't support that)
		else {
			_palPALFormat = kPalRaw;
		}
	}
}

// This just gets the number of colours in the palette
long PaletteFile::getNumColours() {
	// If it is a TPL file, the number is easy to get
	if (_palType == kPalTPL) {
		switch (_dataBuffer[kTPLFormat]) {
			case kTPLRGB:
				return (_size - kTPLData) / 3;
			
			case kTPLNES:
				return (_size - kTPLData);

			case kTPLOther:
				return (_size - kTPLData) / 2;

			default:
				wxLogError("This .tpl file has an unexpected format");
				return 0;
		}

	// If it's a .PAL, it's a little more complicated
	} else {
		switch (_palPALFormat) {
			case kPalPSP:
				return _dataBuffer[kPSPFormat + 1] | _dataBuffer[kPSPFormat];

			case kPalRaw:
				if (_bitdepth < 5) {
					return _size;
				
				} else if (_bitdepth < 8) {
					return _size / 2;
				
				} else {
					return _size / 3;
				}

			default:
				wxLogError("This .pal file has an unexpected format");
				return 0;
		}
	}
}

wxColour PaletteFile::getColour(long index) {
	// If the palette is .tpl and uses the nes palette, we can just grab that easily
	if ((_palType == kPalTPL) && (_dataBuffer[kTPLFormat] == kTPLNES)) {
		return _NESPalette[_dataBuffer[kTPLData + index]];
	}

	// First we need to get the number of bytes from the bitdepth
	float w = 0;
	float n = ((float) (_bitdepth * 3)) / 8; // 8 bits in a byte
	float f = std::modf(n, &w);			 // Separate out the whole number and remainder

	int bytes = (int) w;
	if (f > 0) {
		bytes++;						 // 'Round' by saying that if there is a remainder at all, we count that as a byte
	}

	int byteIndex = index * bytes;
	if (_palType == kPalTPL) {
		byteIndex += kTPLData;
	}

	// We need to extract the red/green/blue bits based on the bitdepth
	int redBits   = _bitdepth;
	int greenBits = _bitdepth;
	int blueBits  = _bitdepth;

	int red   = 0;
	int green = 0;
	int blue  = 0;

	wxByte clrByte = 0;
	wxByte bit = 0;

	// So we need to extract the colour out of the bytes here
	for (int i = 0; i < bytes; i++) {
		clrByte = _dataBuffer[(byteIndex) + i];

		for (int b = 0; b < 8; b++) {
			if (redBits > 0) {
				bit = clrByte & 1;
				bit <<= (_bitdepth - redBits);
				red |= bit;
				redBits--;

			} else if (greenBits > 0) {
				bit = clrByte & 1;
				bit <<= (_bitdepth - greenBits);
				green |= bit;
				greenBits--;
			
			} else if (blueBits > 0) {
				bit = clrByte & 1;
				bit <<= (_bitdepth - blueBits);
				blue |= bit;
				blueBits--;
			}
			clrByte >>= 1;
		}
	}

	// Return the colour
	return wxColour(red, green, blue);
}

void PaletteFile::saveColour(wxColour colour, long index) {
	if ((_palType == kPalTPL) && (_dataBuffer[kTPLFormat] == kTPLNES)) {
		/* To determine the similarity between two colours (so we can find the one most like the chosen colour),
		 * We are going to basically position the colours as points in a 3d colour space, and then find the
		 * distance between those points. The shorter the distance, the more similar they are. This does
		 * not account for *perceived* similarity however, but that's okay for this purpose
		 */

		// We start by setting the potential new colour to 0, and getting the initial distances and similarity values
		int newColour = 0;
		double currentDist = std::sqrt(std::pow(_NESPalette[0].Red() - colour.Red(), 2) + std::pow(_NESPalette[0].Green() - colour.Green(), 2) + std::pow(_NESPalette[0].Blue() - colour.Blue(), 2));
		double maxDist = std::sqrt(std::pow(255, 2) + std::pow(255, 2) + std::pow(255, 2));
		int similarity = (1 - (currentDist / maxDist)) * 100;

		// And then we look at each colour after it, and if the similarity percentage is lower, we make that the new potential colour
		for (int i = 1; i < 64; i++) {
			currentDist = std::sqrt(std::pow(_NESPalette[i].Red() - colour.Red(), 2) + std::pow(_NESPalette[i].Green() - colour.Green(), 2) + std::pow(_NESPalette[i].Blue() - colour.Blue(), 2));
			int newSimilarity = (1 - (currentDist / maxDist)) * 100;
			if (newSimilarity >= similarity) {
				newColour = i;
				similarity = newSimilarity;
			}
		}

		// Now we can just apply the new colour index to the current one
		_dataBuffer[kTPLData + index] = newColour;

	// If it's not an nes palette, we will properly store the colour value
	} else {

		// First we need to get the number of bytes from the bitdepth
		float w = 0;
		float n = ((float) (_bitdepth * 3)) / 8; // 8 bits in a byte
		float f = std::modf(n, &w);			 // Separate out the whole number and remainder

		int bytes = (int) w;
		if (f > 0) {
			bytes++;						 // 'Round' by saying that if there is a remainder at all, we count that as a byte
		}

		int byteIndex = index * bytes;
		if (_palType == kPalTPL) {
			byteIndex += kTPLData;
		}

		int red   = colour.GetRed();
		int green = colour.GetGreen();
		int blue  = colour.GetBlue();

		int bitDepthMax = _bitdepth * 3;
		int bit = 0;

		wxByte clrByte = 0;

		// For every byte in the colour
		for (int i = 0; i < bytes; i++) {
			clrByte = 0;

			// For every bit in the byte
			for (int b = 0; b < 8; b++) {
				// If we're in the first third of the bits, it's red
				if (bitDepthMax > (_bitdepth * 2)) {
					bit = (red & 1) << b;
					red >>= 1;

				// Second third is green
				} else if (bitDepthMax > _bitdepth) {
					bit = (green & 1) << b;
					green >>= 1;
				
				// Last third is blue
				} else {
					bit = (blue & 1) << b;
					blue >>= 1;
				}

				// If we are still adding colour bits, we add the bit to the colour byte
				if (bitDepthMax > 0) {
					bitDepthMax--;
					clrByte |= bit;
				}
			}
			_dataBuffer[byteIndex + i] = clrByte;
		}
	}
}

/* Gfx functions */
long GfxFile::getNumTiles() {
	// First we need to know how many bytes hold one tile of information
	// One tile is 8x8 = 64 pixels
	float w = 0;
	float n = ((float) (_bitsPerPixel * 64)) / 8;
	float f = std::modf(n, &w);

	int numBytes = (int) w;
	if (f > 0) {
		numBytes++;
	}

	return _size / numBytes;
}

wxImage *GfxFile::getTile(long offset, int paletteStart) {
	int brightness = 8 - _bitsPerColour;

	int yOffset = 0;
	int byte = 0;
	int nyble = 0;
	int bit = 0;
	int index = 0;

	wxImage *tile = new wxImage(8, 8, true);

	// Before we create a tile, we need to know if there's enough data left to do so
	float w = 0;
	float n = ((float) (_bitsPerPixel * 64)) / 8;
	float f = std::modf(n, &w);

	int numBytes = (int) w;
	if (f > 0) {
		numBytes++;
	}

	// So we check if the offset + the size of one tile is more than the size of the data
	if ((offset + numBytes) > _size) {
		// If so, just return the blank tile
		return tile;
	}

	// Planar gfx split the gfx data into separate bitplanes, each being a binary representation
	if (_gfxType == kGfxPlanar || _gfxType == kGfxPlanarComp) {

		int composite = (_gfxType == kGfxPlanarComp) ? 2 : 1;
		int yInc = 0;
		int rowInc = 0;
		// Handle odd numbered composite bitdepths here

		// the bits in a bitplane represent a tile in the same order you see the pixels
		for (int y = 0; y < 8; y++) {

			// Which, counter intuitively, means they are stored from most to least significant
			for (int x = 7; x >= 0; x--) {

				index = 0;

				// For every pixel, we have anywhere from 1 to 8 bitplanes to combine
				for (int bp = 0; bp < _bitsPerPixel; bp++) {

					// And now where the magic happens (and by magic, I mean code that doesn't require any conditionals)
					yInc = y * composite;
					rowInc = (bp / composite) * (8 * composite);
					yOffset = (bp & 1) * (composite - 1);

					// With the 3 increment factors calculated, we can add them to byte offset and get the byte from the rom
					byte = getByte(offset + yInc + rowInc + yOffset);

					// Now we can extract the bit at the X position within the byte
					bit = (byte & (1 << x)) >> x;
					bit <<= bp;

					// And add it to the index, which will be our pixel
					index |= bit;
				}

				index += ((std::pow(2, _bitsPerPixel) - 1) * paletteStart);

				// Finally, the pixel is placed at x,y, and uses the pixel as an index to get a colour from the current palette
				tile->SetRGB((7 - x), y, _gfxPalette[index].Red() << brightness, _gfxPalette[index].Green() << brightness, _gfxPalette[index].GetBlue() << brightness);
			}
		}

	} else if (_gfxType == kGfxLinear || _gfxType == kGfxLinearRev) {
		for (int y = 0; y < 8; y++) {
			for (int x = 7; x >= 0; x--) {

				// This is just, super complicated tbh
				nyble = 2 - ((_bitsPerPixel - 1) / 4);
				yOffset = (((x & 1) * 4) * (nyble - 1));
				byte = getByte(offset + (((y * 8) + x) / nyble));

				// Ultimately I'd like for this to somehow not require a conditional, for optimal speed
				if (_gfxType == kGfxLinearRev) {
					// But for now, we are just going to check if this is reversed and if so flip the byte
					byte = ((byte & 0x0F) << 4) | ((byte & 0xF0) >> 4);
				}

				// And then we do a bunch of math that I should probably document at some point
				index = (byte & ((((int) pow(2, _bitsPerPixel) - 1) << ((8 / nyble) - _bitsPerPixel)) << yOffset)) >> yOffset;

				// And this time, we don't have to reverse the pixel drawing order
				tile->SetRGB(x, y, _gfxPalette[index].Red() << brightness, _gfxPalette[index].Green() << brightness, _gfxPalette[index].GetBlue() << brightness);
			}
		}
	
	} else {
		for (int y = 0; y < 8; y++) {
			for (int x = 7; x >= 0; x--) {
				int byteIndex = ((y * 8) + x) * numBytes;

				int redBits   = _bitsPerPixel;
				int greenBits = _bitsPerPixel;
				int blueBits  = _bitsPerPixel;

				int red   = 0;
				int green = 0;
				int blue  = 0;

				wxByte clrByte = 0;
				wxByte bit = 0;

				// So we need to extract the colour out of the bytes here
				for (int i = 0; i < numBytes; i++) {
					clrByte = getByte(byteIndex + i);

					for (int b = 0; b < 8; b++) {
						if (redBits > 0) {
							bit = clrByte & 1;
							bit <<= (_bitsPerPixel - redBits);
							red |= bit;
							redBits--;

						} else if (greenBits > 0) {
							bit = clrByte & 1;
							bit <<= (_bitsPerPixel - greenBits);
							green |= bit;
							greenBits--;
						
						} else if (blueBits > 0) {
							bit = clrByte & 1;
							bit <<= (_bitsPerPixel - blueBits);
							blue |= bit;
							blueBits--;
						}
						clrByte >>= 1;
					}
				}

				tile->SetRGB(x, y, red << brightness, green << brightness, blue << brightness);
			}
		}
	}

	return tile;
}

void GfxFile::setTile(wxImage tile, long offset, int paletteStart) {
	int brightness = 8 - _bitsPerColour;

	int yOffset = 0;
	int byte = 0;
	int nyble = 0;
	int bit = 0;
	int index = 0;

	// Planar gfx split the gfx data into separate bitplanes, each being a binary representation
	if (_gfxType == kGfxPlanar || _gfxType == kGfxPlanarComp) {

		int composite = (_gfxType == kGfxPlanarComp) ? 2 : 1;
		int yInc = 0;
		int rowInc = 0;
		// Handle odd numbered composite bitdepths here

		// the bits in a bitplane represent a tile in the same order you see the pixels
		for (int y = 0; y < 8; y++) {

			// Which, counter intuitively, means they are stored from most to least significant
			for (int x = 7; x >= 0; x--) {

				// First thing is that we need the pixel data from the image
				wxColour pixelColour = wxColour(tile.GetRed(x, y), tile.GetGreen(x, y), tile.GetBlue(x, y));
				int colourIndex = 0;

				// Now we need to find what index that is
				// Make this a while loop you dummy
				for (int i = ((std::pow(2, _bitsPerPixel) - 1) * paletteStart); i < ((std::pow(2, _bitsPerPixel) - 1) * paletteStart) + (std::pow(2, _bitsPerPixel) - 1); i++) {
					wxColour paletteColour = wxColour(_gfxPalette[i].GetRed() << brightness, _gfxPalette[i].GetGreen() << brightness, _gfxPalette[i].GetBlue() << brightness);
					if (pixelColour == paletteColour) {
						colourIndex = i;
						break;
					}
				}

				colourIndex -= ((std::pow(2, _bitsPerPixel) - 1) * paletteStart);

				// Now that we have the index to use, we need to repack it into the gfx data

				// For every pixel, we have anywhere from 1 to 8 bitplanes to combine
				for (int bp = 0; bp < _bitsPerPixel; bp++) {

					// And now where the magic happens (and by magic, I mean code that doesn't require any conditionals)
					yInc = y * composite;
					rowInc = (bp / composite) * (8 * composite);
					yOffset = (bp & 1) * (composite - 1);

					bit = (colourIndex >> bp) & 1;

					int byteX = 7 - x;

					byte = getByte(offset + yInc + rowInc + yOffset);

					if (bit) {
						byte |= (1 << byteX);
					} else {
						byte &= ~(1 << byteX);
					}

					setByte(offset + yInc + rowInc + yOffset, byte);
				}
			}
		}

	} else if (_gfxType == kGfxLinear || _gfxType == kGfxLinearRev) {
		nyble = 2 - ((_bitsPerPixel - 1) / 4);

		for (int y = 0; y < 8; y++) {
			for (int x = 7; x >= 0; x--) {
				// First thing is that we need the pixel data from the image
				wxColour pixelColour = wxColour(tile.GetRed(x, y), tile.GetGreen(x, y), tile.GetBlue(x, y));
				int colourIndex = 0;

				// Now we need to find what index that is
				// Make this a while loop you dummy
				for (int i = ((std::pow(2, _bitsPerPixel)) * paletteStart); i < ((std::pow(2, _bitsPerPixel)) * paletteStart) + (std::pow(2, _bitsPerPixel)); i++) {
					wxColour paletteColour = wxColour(_gfxPalette[i].GetRed() << brightness, _gfxPalette[i].GetGreen() << brightness, _gfxPalette[i].GetBlue() << brightness);
					if (pixelColour == paletteColour) {
						colourIndex = i;
						break;
					}
				}

				colourIndex -= ((std::pow(2, _bitsPerPixel)) * paletteStart);

				nyble = 2 - ((_bitsPerPixel - 1) / 4);
				yOffset = (((x & 1) * 4) * (nyble - 1));

				byte = getByte(offset + (((y * 8) + x) / nyble));

				byte = (byte & ~((((int) pow(2, _bitsPerPixel) - 1) << ((8 / nyble) - _bitsPerPixel)) << yOffset)) | (colourIndex << yOffset);

				if (_gfxType == kGfxLinearRev) {
					byte = ((byte & 0x0F) << 4) | ((byte & 0xF0) >> 4);
				}

				setByte(offset + (((y * 8) + x) / nyble), byte);
			}
		}
	
	} else {
		// First we need to get the number of bytes from the bitdepth
		float w = 0;
		float n = ((float) (_bitsPerPixel * 64)) / 8;
		float f = std::modf(n, &w);

		int numBytes = (int) w;
		if (f > 0) {
			numBytes++;
		}

		for (int y = 0; y < 8; y++) {
			for (int x = 7; x >= 0; x--) {
				int byteIndex = ((y * 8) + x) * numBytes;

				int red   = tile.GetRed(x, y);
				int green = tile.GetGreen(x, y);
				int blue  = tile.GetBlue(x, y);

				int bitDepthMax = _bitsPerPixel * 3;
				int bit = 0;

				wxByte clrByte = 0;

				// For every byte in the colour
				for (int i = 0; i < numBytes; i++) {
					clrByte = 0;

					// For every bit in the byte
					for (int b = 0; b < 8; b++) {
						// If we're in the first third of the bits, it's red
						if (bitDepthMax > (_bitsPerPixel * 2)) {
							bit = (red & 1) << b;
							red >>= 1;

						// Second third is green
						} else if (bitDepthMax > _bitsPerPixel) {
							bit = (green & 1) << b;
							green >>= 1;
						
						// Last third is blue
						} else {
							bit = (blue & 1) << b;
							blue >>= 1;
						}

						// If we are still adding colour bits, we add the bit to the colour byte
						if (bitDepthMax > 0) {
							bitDepthMax--;
							clrByte |= bit;
						}
					}
					setByte(byteIndex + i, clrByte);
				}
			}
		}
	}
}






















