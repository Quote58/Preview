#include "gfxView.h"

wxImage *GfxViewTable::GetTile(int row, int col) {
	// First we need to know how many bytes hold one tile of information
	// One tile is 8x8 = 64 pixels
	float w = 0;
	float n = ((float) (_gfxFile->_bitsPerPixel * 64)) / 8;
	float f = std::modf(n, &w);

	int numBytes = (int) w;
	if (f > 0) {
		numBytes++;
	}

	long offset = (row * (16 * numBytes)) + (numBytes * col);
	return _gfxFile->getTile(offset, _viewBox->_paletteTable->_paletteStart);
}

void GfxViewTable::SetTile(int row, int col, const wxImage &value) {
	float w = 0;
	float n = ((float) (_gfxFile->_bitsPerPixel * 64)) / 8;
	float f = std::modf(n, &w);

	int numBytes = (int) w;
	if (f > 0) {
		numBytes++;
	}

	long offset = (row * (16 * numBytes)) + (numBytes * col);
	_gfxFile->setTile(value, offset, _viewBox->_paletteTable->_paletteStart);
}

void GfxViewTable::onUpArrow(wxCommandEvent &event) {
	if (_viewBox->_paletteTable->_paletteStart > 0) {
		_viewBox->_paletteTable->_paletteStart--;
		GetView()->Refresh();
		_viewBox->_paletteTable->GetView()->Refresh();
	}
}

void GfxViewTable::onDownArrow(wxCommandEvent &event) {
	if (_viewBox->_paletteTable->_paletteStart < 15) {
		_viewBox->_paletteTable->_paletteStart++;
		GetView()->Refresh();
		_viewBox->_paletteTable->GetView()->Refresh();
	}
}

void GfxViewTable::onSaveFile(wxCommandEvent &event) {
	_gfxFile->saveToFile();
}

void GfxViewTable::onBPPChanged(wxSpinEvent &event) {
	_gfxFile->_bitsPerPixel = event.GetPosition();
	GetView()->ForceRefresh();
}

void GfxViewTable::onBPCChanged(wxSpinEvent &event) {
	_gfxFile->_bitsPerColour = event.GetPosition();
	GetView()->ForceRefresh();
}

void GfxViewTable::onTypeChanged(wxCommandEvent &event) {
	_gfxFile->_gfxType = event.GetSelection();
	GetView()->ForceRefresh();
}

void GfxViewTable::onLoadPalette(wxCommandEvent &event) {
	// Prompt the user to open a palette file
	if (wxGetTopLevelParent(GetView()) == nullptr) {
		return;
	}
	
	wxFileDialog open(wxGetTopLevelParent(GetView()), _("Open Palette file"), "", "", "", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	// If they decide to cancel, just return
	if (open.ShowModal() == wxID_CANCEL) {
		return;
	}

	wxString path = open.GetPath();

	if (!wxFile::Exists(path)) {
		wxLogError("Cannot open file");
		return;
	}

	PaletteFile *paletteFile = new PaletteFile(path);

	for (int i = 0; i < paletteFile->getNumColours(); i++) {
		_gfxFile->_gfxPalette[i] = paletteFile->getColour(i);
	}

	if (paletteFile->_palType == 0) {
		switch (paletteFile->_palTPLFormat) {
			case 0:
			case 1:
				_bitsPerColour->SetValue(8);
				_gfxFile->_bitsPerColour = 8;
				break;
			case 2:
				_bitsPerColour->SetValue(5);
				_gfxFile->_bitsPerColour = 5;
				break;
		}
	}

	GetView()->ForceRefresh();
}

/* Renderer */
void GfxViewRenderer::Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected) {
	// We need the table of the grid first
	GfxViewTable *table = (GfxViewTable *) grid.GetTable();

	// If the cell is not the start of a tile, we want to use the string renderer to show the generic 'not part of the rom' symbol
	if (table->GetValue(row, col) == "><") {
		wxColour clr = wxColour(0,0,0,0);
		dc.SetBackgroundMode( wxBRUSHSTYLE_SOLID );
		dc.SetBrush(clr);
		dc.SetPen( *wxTRANSPARENT_PEN );
		dc.DrawRectangle(rect);

	// If the cell is currently selected, just paint it the selection colour instead of drawing gfx
	} else if (grid.IsThisEnabled() && isSelected && grid.HasFocus()) {
		wxColour clr = grid.GetSelectionBackground();
		dc.SetBackgroundMode( wxBRUSHSTYLE_SOLID );
		dc.SetBrush(clr);
		dc.SetPen( *wxTRANSPARENT_PEN );
		dc.DrawRectangle(rect);

	} else {
		wxImage *tile = table->GetTile(row, col);
		wxBitmap *gfx = new wxBitmap(tile->Scale(rect.width, rect.height, wxIMAGE_QUALITY_NORMAL));
		dc.DrawBitmap(*gfx, rect.x, rect.y, false);
	}
}

/* Image Panel */
GfxViewBox::GfxViewBox(wxPanel *parent, wxSlider *rotation, wxButton *rot45, wxButton *rot90, wxButton *rot180, wxButton *mirrorH, wxButton *mirrorV) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(256,256), 0, wxEmptyString) {
	_rotation = rotation;
	_rot45 = rot45;
	_rot90 = rot90;
	_rot180 = rot180;
	_mirrorH = mirrorH;
	_mirrorV = mirrorV;

	// We start by making a blank image as the default tile
	_image = new wxImage(8, 8, true);
	_imageW = 8;
	_imageH = 8;
	_aspectRatio = (double) (_imageW) / (double) _imageH;

	// We bind the paint events to the DC
	Bind(wxEVT_PAINT, &GfxViewBox::paintEvent, this);
	Bind(wxEVT_SIZE, &GfxViewBox::onSize, this);

	// And the mouse events to the DC
	Bind(wxEVT_LEFT_DOWN, &GfxViewBox::onLeftDown, this);
	Bind(wxEVT_LEFT_UP, &GfxViewBox::onLeftUp, this);
	Bind(wxEVT_MOTION, &GfxViewBox::onMouseMotion, this);

	// Rotation controls have their own bound functions
	_rotation->Bind(wxEVT_SLIDER, &GfxViewBox::onRotation, this);
	_rot45->Bind(wxEVT_BUTTON, &GfxViewBox::onRot45, this);
	_rot90->Bind(wxEVT_BUTTON, &GfxViewBox::onRot90, this);
	_rot180->Bind(wxEVT_BUTTON, &GfxViewBox::onRot180, this);
	_mirrorH->Bind(wxEVT_BUTTON, &GfxViewBox::onMirrorH, this);
	_mirrorV->Bind(wxEVT_BUTTON, &GfxViewBox::onMirrorV, this);
}

void GfxViewBox::paintEvent(wxPaintEvent &event) {
	wxPaintDC dc(this);
	render(dc);
}

void GfxViewBox::render(wxDC &dc) {
	// At this point, we may have been given a new image for _image, so we need to re-get the dimentions
	_imageW = _image->GetWidth();
	_imageH = _image->GetHeight();
	_aspectRatio = ((double) _imageW) / ((double) _imageH);

	// We are also going to give it a colour mask for if it gets rotated
	_image->SetMaskColour(_paletteTable->_gfxFile->_gfxPalette[(int) (std::pow(2, _paletteTable->_gfxFile->_bitsPerPixel)) * _paletteTable->_paletteStart].GetRed(),
						  _paletteTable->_gfxFile->_gfxPalette[(int) (std::pow(2, _paletteTable->_gfxFile->_bitsPerPixel)) * _paletteTable->_paletteStart].GetGreen(),
						  _paletteTable->_gfxFile->_gfxPalette[(int) (std::pow(2, _paletteTable->_gfxFile->_bitsPerPixel)) * _paletteTable->_paletteStart].GetBlue());

	// Now we can scale the image
	_scaledW = kPanelSize;
	_scaledH = (int) (_scaledW / _aspectRatio);

	if (_scaledH > kPanelSize) {
		_scaledH = kPanelSize;
		_scaledW = (int) (_scaledH * _aspectRatio);
	}
	_scaledImage = _image->Scale(_scaledW, _scaledH);

	// And finally, we either use the scaled image, or if it's been rotated we scale and use the rotated image
	wxBitmap bitmap;
	if (_angle != 0) {
		bitmap = wxBitmap(_rotatedImage.Scale(_scaledW, _scaledH));
	} else {
		bitmap = wxBitmap(_scaledImage);
	}
    dc.DrawBitmap(bitmap, 0, 0, false);
}

void GfxViewBox::onSize(wxSizeEvent &event) {
	Refresh();
	event.Skip();
}

void GfxViewBox::onLeftDown(wxMouseEvent &event) {
	_dragging = true;

	wxPaintDC dc(this);
	int numW = _imageW / 8;
	int numH = _imageH / 8;

	int pixelW = (_scaledW / numW) / 8;
	int pixelH = (_scaledH / numH) / 8;
	int mouseX = event.GetLogicalPosition(dc).x / pixelW;
	int mouseY = event.GetLogicalPosition(dc).y / pixelH;

	if ((mouseX < _imageW) && (mouseY < _imageH) && (mouseX >= 0) && (mouseY >= 0)) {
		int brightness = 8 - _paletteTable->_gfxFile->_bitsPerColour;
		wxColour colour = _paletteTable->_gfxFile->_gfxPalette[(_paletteTable->_lastRow * 8) + _paletteTable->_lastCol +
															   ((int) (std::pow(2, _paletteTable->_gfxFile->_bitsPerPixel)) * _paletteTable->_paletteStart)];
		_image->SetRGB(mouseX, mouseY, colour.Red() << brightness, colour.Green() << brightness, colour.Blue() << brightness);
	}

	setImages(numW, numH, *_image);

	Refresh();
	event.Skip();
}

void GfxViewBox::onMouseMotion(wxMouseEvent &event) {
	if (_dragging == true) {
		onLeftDown(event);
	}
}

void GfxViewBox::onLeftUp(wxMouseEvent &event) {
	_dragging = false;
}

void GfxViewBox::onRot45(wxCommandEvent &event) {
	double rad45 = 45 * (M_PI / 180);
	rotate(rad45);
	*_image = _rotatedImage;
}

void GfxViewBox::onRot90(wxCommandEvent &event) {
	double rad90 = 90 * (M_PI / 180);
	rotate(rad90);
	*_image = _rotatedImage;
}

void GfxViewBox::onRot180(wxCommandEvent &event) {
	_rotatedImage = _image->Rotate180();
	setImages(_imageW / 8, _imageH / 8, _rotatedImage);
	Refresh();
	*_image = _rotatedImage;
}

void GfxViewBox::onMirrorH(wxCommandEvent &event) {
	_rotatedImage = _image->Mirror(true);
	setImages(_imageW / 8, _imageH / 8, _rotatedImage);
	Refresh();
	*_image = _rotatedImage;
}

void GfxViewBox::onMirrorV(wxCommandEvent &event) {
	_rotatedImage = _image->Mirror(false);
	setImages(_imageW / 8, _imageH / 8, _rotatedImage);
	Refresh();
	*_image = _rotatedImage;
}

void GfxViewBox::onRotation(wxCommandEvent &event) {
	double radAngle = event.GetInt() * (M_PI / 180);
	rotate(radAngle);
	_angle = event.GetInt();
}

void GfxViewBox::rotate(double angle) {
	_rotatedImage = _image->Rotate(angle, wxDefaultPosition, false);

	int rotWidth = _rotatedImage.GetWidth();
	int rotHeight = _rotatedImage.GetHeight();

	// We want to crop evenly on all sides, so we need to know how far in from 0, based on the difference between the images
	int cropX = std::max(0, (rotWidth - _imageW) / 2);
	int cropY = std::max(0, (rotHeight - _imageH) / 2);

	// And we want the size of the cropped image to be whichever image is smaller (which is probably always the original)
	int cropWidth = std::min(_imageW, rotWidth);
	int cropHeight = std::min(_imageH, rotHeight);

	// And now rotatedImage should be the original size
	_rotatedImage = _rotatedImage.GetSubImage(wxRect(cropX, cropY, cropWidth, cropHeight));
	if ((rotWidth < _imageW) || (rotHeight < _imageH)) {
		wxImage *newImage = new wxImage(_imageW, _imageH);
		newImage->Paste(_rotatedImage, std::max(0, (_imageW - rotWidth) / 2), std::max(0, (_imageH - rotHeight) / 2));
		_rotatedImage = *newImage;
	}

	setImages(_imageW / 8, _imageH / 8, _rotatedImage);
	Refresh();
}

void GfxViewBox::setImages(int numW, int numH, wxImage image) {
	float w = 0;
	float n = ((float) (_paletteTable->_gfxFile->_bitsPerPixel * 64)) / 8;
	float f = std::modf(n, &w);

	int numBytes = (int) w;
	if (f > 0) {
		numBytes++;
	}

	for (int y = 0; y < numH; y++) {
		for (int x = 0; x < numW; x++) {
			wxImage newTile = image.GetSubImage(wxRect(x * 8, y * 8, 8, 8));
			long offset = ((_paletteTable->_gfxFile->_lastTop + y) * (16 * numBytes)) + (numBytes * (_paletteTable->_gfxFile->_lastLeft + x));	
			_paletteTable->_gfxFile->setTile(newTile, offset, _paletteTable->_paletteStart);
		}
	}
}

/* Palette Grid */
// Get a colour for a given row/col of the palette grid
wxColour GfxPaletteTable::GetColour(int row, int col) {
	// Colour to get is the row * row size + column
	long colourNum = ((row * 8) + col) + ((std::pow(2, _gfxFile->_bitsPerPixel)) * _paletteStart);

	// If there are fewer colours than the grid square chozen, return black
	if (colourNum > 256) {
		return wxColour(0,0,0,0);
	
	// Otherwise, return the colour from the colour file for this index
	} else {
		return _gfxFile->_gfxPalette[colourNum];
	}
}

// Set a colour for a given row/col of the palette grid
void GfxPaletteTable::SetColour(int row, int col, const wxColour &value) {
	long colourNum = ((row * 8) + col) + ((std::pow(2, _gfxFile->_bitsPerPixel)) * _paletteStart);
	_gfxFile->_gfxPalette[colourNum] = value;
}

// Render the data as a colour for any given cell of the grid
void GfxPaletteRenderer::Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected) {
	dc.SetBackgroundMode(wxBRUSHSTYLE_SOLID);

	wxColour clr;
	if (grid.IsThisEnabled()) {
		if (isSelected) {
			if (grid.HasFocus()) {
				clr = grid.GetSelectionBackground();

			} else {
				clr = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW);
			}
		
		} else {
			GfxPaletteTable *table = (GfxPaletteTable *) grid.GetTable();

			if (table->GetValue(row, col) == "><") {
				clr = wxColour(0,0,0,0);
				return;
			
			} else {
				int brightness = 8 - table->_gfxFile->_bitsPerColour;
				clr = table->GetColour(row, col);
				clr = wxColour(clr.Red() << brightness, clr.Green() << brightness, clr.Blue() << brightness);
			}
		}
	
	} else {
		// grey out fields if the grid is disabled
		clr = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
	}

	dc.SetBrush(clr);
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.DrawRectangle(rect);
}
