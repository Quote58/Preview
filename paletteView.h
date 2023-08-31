// PaletteView

#ifndef PALETTE_H
#define PALETTE_H

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

/* --- Palette View ---
 * The palette view is made of a grid that displays data
 * as solid colours. The grid is technically made of
 * strings, but when it displays a given square, it grabs
 * a colour value from the palette file, using a custom
 * table that has a palette file.
 *
 * *** Realistically, this should actually be a special
 * type of grid instead of piggybacking off of strings ***
 */

class PaletteViewRenderer : public wxGridCellStringRenderer {
public:
    virtual void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected) wxOVERRIDE;
};

class PaletteViewTable : public wxGridTableBase {
public:
	PaletteFile *_paletteFile = nullptr;
	wxButton *_saveFile = nullptr;
	wxSpinCtrl *_bitdepth = nullptr;
	wxSpinCtrl *_rSpin = nullptr;
	wxSpinCtrl *_gSpin = nullptr;
	wxSpinCtrl *_bSpin = nullptr;
	wxSpinCtrl *_nSpin = nullptr;

	// These represent the currently selected colour
	int _lastRow = 0;
	int _lastCol = 0;

	PaletteViewTable(wxString filePath, wxButton *saveFile, wxSpinCtrl *bitdepth, wxSpinCtrl *rSpin, wxSpinCtrl *gSpin, wxSpinCtrl *bSpin, wxSpinCtrl *nSpin) {
		_paletteFile = new PaletteFile(filePath);
		_saveFile = saveFile;
		_bitdepth = bitdepth;
		_bitdepth->SetValue(_paletteFile->_bitdepth);
		_rSpin = rSpin;
		_gSpin = gSpin;
		_bSpin = bSpin;
		_nSpin = nSpin;

		if ((_paletteFile->_palType == 0) && (_paletteFile->_palTPLFormat == 1)) {
			// The cool thing is that when you hide these, the 'label' becomes RGB for nSpin :D
			_rSpin->Hide();
			_gSpin->Hide();
			_bSpin->Hide();

		// If the palette is not NES, then show the full 3 colour spinners
		} else {
			_nSpin->Hide();
		}

		_rSpin->SetRange(1, std::pow(2, _paletteFile->_bitdepth) - 1);
		_gSpin->SetRange(1, std::pow(2, _paletteFile->_bitdepth) - 1);
		_bSpin->SetRange(1, std::pow(2, _paletteFile->_bitdepth) - 1);

		_saveFile->Bind(wxEVT_BUTTON, &PaletteViewTable::onSaveFile, this);
		_bitdepth->Bind(wxEVT_SPINCTRL, &PaletteViewTable::onBitdepthChanged, this);
		_rSpin->Bind(wxEVT_SPINCTRL, &PaletteViewTable::onRedChanged, this);
		_gSpin->Bind(wxEVT_SPINCTRL, &PaletteViewTable::onGreenChanged, this);
		_bSpin->Bind(wxEVT_SPINCTRL, &PaletteViewTable::onBlueChanged, this);
		_nSpin->Bind(wxEVT_SPINCTRL, &PaletteViewTable::onNESChanged, this);
	}

	// These are simple enough to include here
	int GetNumberRows() wxOVERRIDE {
		if ((_paletteFile->getNumColours() / 16) == 0) {
			return 1;

		} else {
			return _paletteFile->getNumColours() / 16;
		}
	}

	// We will always be using 16 columns <- or maybe not??
	int GetNumberCols() wxOVERRIDE { return 16; }

	wxString GetValue(int row, int col) wxOVERRIDE {
		// data to get is the row * row size + column
		long num = (row * 16) + col;

		// If there are fewer colours than the grid square chozen, return a special string
		if (num > _paletteFile->getNumColours()) {
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
	void onSaveFile(wxCommandEvent &event);
	void onBitdepthChanged(wxSpinEvent &event);
	void onRedChanged(wxSpinEvent &event);
	void onGreenChanged(wxSpinEvent &event);
	void onBlueChanged(wxSpinEvent &event);
	void onNESChanged(wxSpinEvent &event);
};

#endif