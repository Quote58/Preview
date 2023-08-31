// GfxView

#ifndef GFX_H
#define GFX_H

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include "file.h"
#include "wx/grid.h"
#include "wx/generic/gridctrl.h"
#include "wx/generic/grideditors.h"
#include <wx/spinctrl.h>
#include <math.h>

/* --- Graphics View ---
 * 
 */

// We also need a version of the palette grid for showing the current palette
class GfxPaletteRenderer : public wxGridCellStringRenderer {
public:
    virtual void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected) wxOVERRIDE;
};

class GfxPaletteTable : public wxGridTableBase {
public:
	GfxFile *_gfxFile = nullptr;

	int _paletteStart = 0;

	// These represent the currently selected colour
	int _lastRow = 0;
	int _lastCol = 0;

	GfxPaletteTable(GfxFile *gfxFile) {
		_gfxFile = gfxFile;

	}

	// We will always be using 2 rows
	int GetNumberRows() wxOVERRIDE {
		return 2;
	}

	// We will always be using 8
	int GetNumberCols() wxOVERRIDE {
		return 8;
	}

	wxString GetValue(int row, int col) wxOVERRIDE {
		// data to get is the row * row size + column
		long num = (row * 8) + col;

		// If there are fewer colours than the grid square chozen, return a special string
		if (num > 256) {
			return "><";
		
		// Otherwise, return nothing since this data isn't used for the colour itself
		} else {
			return "";
		}
	}

	void SetValue(int row, int col, const wxString &value) wxOVERRIDE {}
	bool IsEmptyCell(int row, int col) wxOVERRIDE { return false; }

	// These are the ones we actually will be using
	wxColour GetColour(int row, int col);
	void SetColour(int row, int col, const wxColour &value);
};

// We need an image panel for the zoomed in version of the tile
class GfxViewBox : public wxPanel {
public:
	const int kPanelSize = 256;

	// First we have the raw pixels inserted into a wxImage
	wxImage *_image = nullptr;

	// Then we have the image as it has been scaled to the 256x256 DC
	wxImage _scaledImage;

	// And lastly we have the rotated image, which is the scaled image rotated by X degrees
	wxImage _rotatedImage;
	GfxPaletteTable *_paletteTable = nullptr;
	wxSlider *_rotation = nullptr;
	wxButton *_rot45 = nullptr;
	wxButton *_rot90 = nullptr;
	wxButton *_rot180 = nullptr;
	wxButton *_mirrorH = nullptr;
	wxButton *_mirrorV = nullptr;
	int _imageW = 0;
	int _imageH = 0;
	int _scaledW = 0;
	int _scaledH = 0;
	int _angle = 0;
	double _aspectRatio = 0;
	bool _dragging = false;

	GfxViewBox(wxPanel *parent, wxSlider *rotation, wxButton *rot45, wxButton *rot90, wxButton *rot180, wxButton *mirrorH, wxButton *mirrorV);

	void paintEvent(wxPaintEvent &event);
	void onSize(wxSizeEvent &event);
	void render(wxDC &dc);
	void onLeftDown(wxMouseEvent &event);
	void onMouseMotion(wxMouseEvent &event);
	void onLeftUp(wxMouseEvent &event);
	void onRot45(wxCommandEvent &event);
	void onRot90(wxCommandEvent &event);
	void onRot180(wxCommandEvent &event);
	void onMirrorH(wxCommandEvent &event);
	void onMirrorV(wxCommandEvent &event);
	void onRotation(wxCommandEvent &event);
	void rotate(double angle);
	void setImages(int numW, int numH, wxImage image);
};


class GfxViewRenderer : public wxGridCellStringRenderer {
public:
    virtual void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected) wxOVERRIDE;
};

class GfxViewTable : public wxGridTableBase {
public:
	GfxFile *_gfxFile = nullptr;
	wxButton *_saveFile = nullptr;
	wxSpinCtrl *_bitsPerPixel = nullptr;
	wxSpinCtrl *_bitsPerColour = nullptr;
	wxChoice *_gfxType = nullptr;
	wxButton *_loadPal = nullptr;
	GfxViewBox *_viewBox = nullptr;
	wxButton *_upArrow = nullptr;
	wxButton *_downArrow = nullptr;
	int _numBytes = 0;

	int _lastTop = -1;
	int _lastLeft = -1;
	int _lastRight = -1;
	int _lastBottom = -1;

	GfxViewTable(wxString filePath, wxButton *saveFile, wxSpinCtrl *bitsPerPixel, wxSpinCtrl *bitsPerColour, wxChoice *gfxType,
				 wxButton *loadPalette, GfxViewBox *viewBox, wxButton *upArrow, wxButton *downArrow) {
		_gfxFile = new GfxFile(filePath);
		_saveFile = saveFile;
		_bitsPerPixel = bitsPerPixel;
		_bitsPerPixel->SetValue(_gfxFile->_bitsPerPixel);
		_bitsPerColour = bitsPerColour;
		_bitsPerColour->SetValue(_gfxFile->_bitsPerColour);
		_gfxType = gfxType;
		_gfxType->SetSelection(_gfxFile->_gfxType);
		_loadPal = loadPalette;
		_viewBox = viewBox;
		_upArrow = upArrow;
		_downArrow = downArrow;

		_upArrow->Bind(wxEVT_BUTTON, &GfxViewTable::onUpArrow, this);
		_downArrow->Bind(wxEVT_BUTTON, &GfxViewTable::onDownArrow, this);
		_bitsPerPixel->Bind(wxEVT_SPINCTRL, &GfxViewTable::onBPPChanged, this);
		_bitsPerColour->Bind(wxEVT_SPINCTRL, &GfxViewTable::onBPCChanged, this);
		_gfxType->Bind(wxEVT_CHOICE, &GfxViewTable::onTypeChanged, this);
		_loadPal->Bind(wxEVT_BUTTON, &GfxViewTable::onLoadPalette, this);
		_saveFile->Bind(wxEVT_BUTTON, &GfxViewTable::onSaveFile, this);
	}

	// These are simple enough to include here
	int GetNumberRows() wxOVERRIDE {
		if ((_gfxFile->getNumTiles() / 16) == 0) {
			return 1;

		} else {
			return _gfxFile->getNumTiles() / 16;
		}
	}

	// We will always be using 16 columns
	int GetNumberCols() wxOVERRIDE { return 16; }

	wxString GetValue(int row, int col) wxOVERRIDE {
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

		// If there are fewer colours than the grid square chozen, return a special string
		if (offset > _gfxFile->_size) {
			return "><";
		
		// Otherwise, return nothing since this data isn't used for the colour itself
		} else {
			return "";
		}
	}

	void SetValue(int row, int col, const wxString &value) wxOVERRIDE {}
	bool IsEmptyCell(int row, int col) wxOVERRIDE { return false; }

	// These are the ones we actually will be using
	wxImage *GetTile(int row, int col);
	void SetTile(int row, int col, const wxImage &value);

	void onUpArrow(wxCommandEvent &event);
	void onDownArrow(wxCommandEvent &event);
	void onBPPChanged(wxSpinEvent &event);
	void onBPCChanged(wxSpinEvent &event);
	void onTypeChanged(wxCommandEvent &event);
	void onLoadPalette(wxCommandEvent &event);
	void onSaveFile(wxCommandEvent &event);
};

#endif