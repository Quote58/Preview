#include "textView.h"

TextView::TextView(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style,
				   const wxString& name, wxString filePath, wxButton *saveFile, wxSearchCtrl *searchFile,
				   wxCheckBox *wordWrap, wxChoice *darkMode)
				   : wxStyledTextCtrl(parent, id, pos, size, style, name) {

	_filePath = filePath;
	_saveFile = saveFile;
	_searchFile = searchFile;
	_wordWrap = wordWrap;
	_darkMode = darkMode;

	// And now we bind all the controls to their functions
	_saveFile->Bind(wxEVT_BUTTON, &TextView::onTextFileSave, this);
	_searchFile->Bind(wxEVT_SEARCH, &TextView::onSearchFile, this);
	_wordWrap->Bind(wxEVT_CHECKBOX, &TextView::onWordWrap, this);
	_darkMode->Bind(wxEVT_CHOICE, &TextView::onDarkMode, this);
}

void TextView::setupTextView() {
	wxString fileName = _filePath.AfterLast('/');
	wxString fileExt = fileName.AfterLast('.').Lower();
	_lexer = -1;

	// For the moment we won't deal with Makefiles
	if (fileExt.IsSameAs(fileName) != true) {
		// All the file extensions should be in _lexerType, but just in case
		if (_lexerType.find(fileExt) != _lexerType.end()) {
			// Set the lexer to be the lexer for the given file extension
			SetLexer(_lexerType.at(fileExt));
			_lexer = _lexerType.at(fileExt);
		}
	}

	/* Styling */
	// First thing is to clear all previous styling (for some reason?) and make everything default
	StyleClearAll();

	// Next are the margins
	// First margin being the line numbers
	SetMarginType(MarginLineNumber, wxSTC_MARGIN_NUMBER);
	SetMarginWidth(MarginLineNumber, kMarginSize);

	// We want the line numbers to use a teletype font family so they are consistently aligned
	wxFont lineNumberFont = StyleGetFont(wxSTC_STYLE_LINENUMBER);
	lineNumberFont.SetFamily(wxFONTFAMILY_TELETYPE);
	StyleSetFont(wxSTC_STYLE_LINENUMBER, lineNumberFont);

	// And we want the second margin to be for code folding
	SetMarginType(MarginFold, wxSTC_MARGIN_SYMBOL);
	SetMarginWidth(MarginFold, kMarginSize / 2);
	SetMarginMask(MarginFold, wxSTC_MASK_FOLDERS);
	SetMarginSensitive(MarginFold, true);

	// Which requires these property things
	SetProperty("fold", "1");
	SetProperty("fold.comment", "1");
	SetProperty("fold.compact", "1");

	// And the marker itself in all the configurations (background is the arrow interior, foreground is the outline)
	// Starting with when the folder is closed
	MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_CIRCLEPLUS);
	MarkerSetForeground(wxSTC_MARKNUM_FOLDER, _clrMargin);
	MarkerSetBackground(wxSTC_MARKNUM_FOLDER, wxColour(*wxBLACK));

	// And when the folder is open
	MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_CIRCLEMINUS);
	MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, _clrMargin);
	MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, wxColour(*wxBLACK));

	// Then the line between an open folder
	MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE);
	MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB, _clrMargin);
	MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, wxColour(*wxBLACK));

	// And the end of the line
	MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER);
	MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL, _clrMargin);
	MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, wxColour(*wxBLACK));

	// Next when a sub folder is closed
	MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_CIRCLEPLUSCONNECTED);
	MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, _clrMargin);
	MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, wxColour(*wxBLACK));

	// When a sub folder is open
	MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_CIRCLEMINUSCONNECTED);
	MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, _clrMargin);
	MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, wxColour(*wxBLACK));

	// And the end of the line for a sub folder (which assumes there are lines on either side)
	MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER);
	MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL, _clrMargin);
	MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, wxColour(*wxBLACK));

	// And now we bind the event to the function
	Bind(wxEVT_STC_MARGINCLICK, &TextView::onMarginClick, this);

	// Now we can do the actual styling
	SyntaxHighlight(false);
}

void TextView::onMarginClick(wxStyledTextEvent &event) {
	if (event.GetMargin() == MarginFold) {
		int lineClick = LineFromPosition(event.GetPosition());
		int levelClick = GetFoldLevel(lineClick);

		if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) {
			ToggleFold(lineClick);
		}
	}
}

void TextView::SyntaxHighlight(bool dark) {
	// We start with the margins
	if (dark) {
		StyleSetForeground(wxSTC_STYLE_LINENUMBER, _clrDarkLineNum);
		StyleSetBackground(wxSTC_STYLE_LINENUMBER, _clrDarkMargin);
		SetFoldMarginColour(true, _clrDarkFoldBkg);
		SetFoldMarginHiColour(true, _clrDarkFoldFrg);

	} else {
		StyleSetForeground(wxSTC_STYLE_LINENUMBER, _clrLineNum);
		StyleSetBackground(wxSTC_STYLE_LINENUMBER, _clrMargin);
		SetFoldMarginColour(true, _clrFoldBkg);
		SetFoldMarginHiColour(true, _clrFoldFrg);		
	}

	// For light mode, we want to start out by setting the foreground to black and the background to white
	for (int i = 0; i < 39; i++) {
		if (dark) {
			StyleSetBackground(i, _clrDarkBackground);
			StyleSetForeground(i, _clrDarkText);
		
		} else {
			StyleSetBackground(i, _clrBackground);
			StyleSetForeground(i, _clrText);
		}
	}

	// If the lexer has a defined palette map, use it
	if (_paletteMap.find(_lexer) != _paletteMap.end()) {

		// Loop over the list of defined colour values and apply them
		int index = 0;
		while (_paletteMap.at(_lexer)[index] != wxNullColour) {
			if (dark) {
				// If dark is enabled, grab the equivalent dark colour of the light colour
				StyleSetForeground(index, _lightToDark.at(_paletteMap.at(_lexer)[index]));

			} else {
				StyleSetForeground(index, _paletteMap.at(_lexer)[index]);
			}
			index++;
		}
	}
}

// Dark mode toggle literally just calls syntax highlighting with dark mode enabled
void TextView::onDarkMode(wxCommandEvent &event) {
	if (event.GetSelection() == 1) {
		SyntaxHighlight(true);
	} else {
		SyntaxHighlight(false);
	}
}

// Save is really simple since the text control already has a function for it
void TextView::onTextFileSave(wxCommandEvent &event) {
	SaveFile(_filePath);
}

void TextView::onSearchFile(wxCommandEvent &event) {
	int pos = FindText(0, GetTextLength(), event.GetString());
	GotoPos(pos);
}

// Very simple, just changes word wrapping based on what it currently is set to
void TextView::onWordWrap(wxCommandEvent &event) {
	if (GetWrapMode() == wxSTC_WRAP_WORD) {
		SetWrapMode(wxSTC_WRAP_NONE);
	
	} else {
		SetWrapMode(wxSTC_WRAP_WORD);
	}
}

















