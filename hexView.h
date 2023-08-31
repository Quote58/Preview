// HexView

#ifndef HEX_H
#define HEX_H

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include "file.h"
#include "wx/grid.h"
#include "wx/generic/gridctrl.h"
#include "wx/generic/grideditors.h"

class HexGrid : public wxGrid {
public:
	  wxButton *_saveFile = nullptr;
	wxTextCtrl *_goToCtrl = nullptr;
	wxCheckBox *_gridlines = nullptr;
	  wxChoice *_viewAs = nullptr;

	HexGrid(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name,
			wxButton *saveFile, wxTextCtrl *goToCtrl, wxCheckBox *gridlines, wxChoice *viewAs) : wxGrid(parent, id, pos, size, style, name) {
		
		_saveFile = saveFile;
		_goToCtrl = goToCtrl;
		_gridlines = gridlines;
		_viewAs = viewAs;

		_saveFile->Bind(wxEVT_BUTTON,     &HexGrid::onSaveFile, this);
		_goToCtrl->Bind(wxEVT_TEXT_ENTER, &HexGrid::onGoToEnter, this);
		_gridlines->Bind(wxEVT_CHECKBOX,  &HexGrid::onGridLinesCheck, this);
		_viewAs->Bind(wxEVT_CHOICE, 	  &HexGrid::onViewTypeChanged, this);

		Bind(wxEVT_SCROLLWIN_TOP, 		   &HexGrid::onScrollEnd, this);
		Bind(wxEVT_SCROLLWIN_BOTTOM, 	   &HexGrid::onScrollEnd, this);
		Bind(wxEVT_SCROLLWIN_PAGEUP, 	   &HexGrid::onScrollEnd, this);
		Bind(wxEVT_SCROLLWIN_PAGEDOWN, 	   &HexGrid::onScrollEnd, this);
		Bind(wxEVT_SCROLLWIN_THUMBRELEASE, &HexGrid::onScrollEnd, this);
		Bind(wxEVT_MOUSEWHEEL,			   &HexGrid::onMouseWheel, this);
	}

	void onSaveFile(wxCommandEvent &event);
	void onGoToEnter(wxCommandEvent &event);
	void goToOffset(long offset);
	void onGridLinesCheck(wxCommandEvent &event);
	void onViewTypeChanged(wxCommandEvent &event);
	void onScrollEnd(wxScrollWinEvent &event);
	void onMouseWheel(wxMouseEvent &event);
	void adjustForScroll();

};

class HexViewTable : public wxGridTableBase {
public:
	File *_hexFile = nullptr;

	long _offset = 0;
	bool _viewAsString = false;

	HexViewTable(wxString filePath) {
		_hexFile = new File(filePath);
	}

	// These are simple enough to include here
	int GetNumberRows() wxOVERRIDE {
		if ((_hexFile->_size / 16) == 0) {
			return 1;

		} else {
			return _hexFile->_size / 16;
		}
	}

	// We will always be using 16 columns
	int GetNumberCols() wxOVERRIDE { return 17; }
	wxString GetValue(int row, int col) wxOVERRIDE;
	void SetValue(int row, int col, const wxString &value) wxOVERRIDE;
	bool IsEmptyCell(int row, int col) wxOVERRIDE { return false; }

};

#endif