#ifndef PREVIEW_FILE_H
#define PREVIEW_FILE_H

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/vector.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
#include <wx/spinctrl.h>

/* Preview File handler
 * This class handles the actual I/O
 * for the file being edited
 */
class File {
public:
	File(wxString path);
	~File() {}

	wxFile *_file;							// The file itself
	wxByte *_dataBuffer;					// A mutable buffer of the file data
	wxString _filePath;						// The path to the file
	wxString _name;							// The name of the file
	long _size;

	void saveToFile();						// Replaces the file contents with the dataBuffer contents, ie. Applies the changes

	wxByte getByte(long offset);
	void setByte(long offset, wxByte byte);
};

class PaletteFile : public File {
// Big thank you to creaothceann for the explanation of TPL file format

enum PalType {
	kPalTPL,
	kPalPAL,
	kPalACT
};

enum TPLFormat {
	kTPLRGB,
	kTPLNES,
	kTPLOther
};

enum PalFormat {
	kPalRaw,
	kPalPSP
};

enum PSPOffsets {
	kPSPSig = 0,
	kPSPFormat = 8,
	kPSPData = 10
};

enum TPLOffsets {
	kTPLSig    = 0x0,
	kTPLFormat = 0x3,
	kTPLData   = 0x4
};

public:
	PaletteFile(wxString path);
	~PaletteFile();

	wxColour _NESPalette[64] = {wxColour(112, 112, 112), wxColour(32, 24, 136),   wxColour(0, 0, 168),     wxColour(64, 0, 152),    wxColour(136, 0, 112),   wxColour(168, 0, 16),    wxColour(160, 0, 0),     wxColour(120, 8, 0),
								wxColour(64, 40, 0),     wxColour(0, 64, 0),      wxColour(0, 80, 0),      wxColour(0, 56, 16),     wxColour(24, 56, 88),    wxColour(0, 0, 0),       wxColour(0, 0, 0),       wxColour(0, 0, 0),
								wxColour(184, 184, 184), wxColour(0, 112, 232),   wxColour(32, 56, 232),   wxColour(128, 0, 240),   wxColour(184, 0, 184),   wxColour(224, 0, 88),    wxColour(216, 40, 0),    wxColour(200, 72, 8),
								wxColour(136, 112, 0),   wxColour(0, 144, 0),     wxColour(0, 168, 0),     wxColour(0, 144, 56),    wxColour(0, 128, 136),   wxColour(0, 0, 0),       wxColour(0, 0, 0),       wxColour(0, 0, 0),
								wxColour(248, 248, 248), wxColour(56, 184, 248),  wxColour(88, 144, 248),  wxColour(160, 136, 248), wxColour(240, 120, 248), wxColour(248, 112, 176), wxColour(248, 112, 96),  wxColour(248, 152, 56),
								wxColour(240, 184, 056), wxColour(128, 208, 16),  wxColour(72, 216, 072),  wxColour(88, 248, 152),  wxColour(0, 232, 216),   wxColour(120, 120, 120), wxColour(0, 0, 0),       wxColour(0, 0, 0),
								wxColour(248, 248, 248), wxColour(168, 224, 248), wxColour(192, 208, 248), wxColour(208, 200, 248), wxColour(248, 192, 248), wxColour(248, 192, 216), wxColour(248, 184, 176), wxColour(248, 216, 168),
								wxColour(248, 224, 160), wxColour(224, 248, 160), wxColour(168, 240, 184), wxColour(176, 248, 200), wxColour(152, 248, 240), wxColour(196, 196, 196), wxColour(0, 0, 0),       wxColour(0, 0, 0)};

	int _bitdepth = 5;

	wxVector<wxVector<wxColour>> _copyMatrix;

	int _lastTop = -1;
	int _lastLeft = -1;
	int _lastRight = -1;
	int _lastBottom = -1;

	PalType _palType = kPalTPL;
	TPLFormat _palTPLFormat = kTPLRGB;
	PalFormat _palPALFormat = kPalRaw;

	long getNumColours();								// Just gets the number of colours in the palette
	wxColour getColour(long index);						// Gets the colour in the file at index
	void saveColour(wxColour colour, long index);		// Saves a new colour to a given index
};

class GfxFile : public File {

enum GfxType {
	kGfxPlanar,
	kGfxPlanarComp,
	kGfxLinear,
	kGfxLinearRev,
	kGfxRGB
};

public:
	GfxFile(wxString path) : File(path) {
		wxColour basicPalette[16] = {wxColour(0x00,0x00,0x00), wxColour(0x02,0x02,0x02), wxColour(0x04,0x04,0x04), wxColour(0x05,0x05,0x05),
									 wxColour(0x07,0x07,0x07), wxColour(0x08,0x08,0x08), wxColour(0x0A,0x0A,0x0A), wxColour(0x0B,0x0B,0x0B), 
									 wxColour(0x0D,0x0D,0x0D), wxColour(0x0F,0x0F,0x0F), wxColour(0x11,0x11,0x11), wxColour(0x13,0x13,0x13),
									 wxColour(0x15,0x15,0x15), wxColour(0x18,0x18,0x18), wxColour(0x1B,0x1B,0x1B), wxColour(0x1F,0x1F,0x1F)};

		// The basic palette just needs to be a 16 colour gradient 16 times to fill out the full 256 colours
		for (int i = 0; i < 256; i++) {
			_gfxPalette[i] = basicPalette[i % 16];
		}
	}
	
	~GfxFile();

	int _bitsPerPixel = 4;
	int _bitsPerColour = 5;
	int _gfxType = kGfxPlanarComp;

	wxColour _gfxPalette[256];

	wxVector<wxVector<wxImage>> _copyMatrix;

	int _lastTop = -1;
	int _lastLeft = -1;
	int _lastRight = -1;
	int _lastBottom = -1;

	long getNumTiles();
	wxImage *getTile(long offset, int paletteStart);
	void setTile(wxImage tile, long index, int paletteStart);
};

class PatchFile : public File {

};


class BitsFile : public File {

};

#endif