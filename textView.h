// Text View

#ifndef TEXT_H
#define TEXT_H

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include "file.h"
#include <wx/stc/stc.h>
#include <wx/srchctrl.h>
#include <wx/fontpicker.h>
#include <unordered_map>

enum Margin {
	MarginLineNumber,
	MarginFold
};

enum CommonTextValues {
	kMarginSize = 35
};

class TextView : public wxStyledTextCtrl {
public:
	// To be able to have a map of wxColour : wxColour, we need a custom hash function
	struct ColourHash {
		std::size_t operator()(const wxColour &colour) const {
			// Really simple, just combine the values of all the colours
			return std::hash<int>()(colour.GetRed() ^ colour.GetGreen() ^ colour.GetBlue());
		}
	};

	// We need the filepath for saving
	wxString _filePath;

	int _lexer = -1;

	/* Common colours for styling */
	// Light mode
	wxColour _clrText = *wxBLACK;
	wxColour _clrBackground = *wxWHITE;
	wxColour _clrMargin = wxColour(250,250,250);
	wxColour _clrLineNum = *wxLIGHT_GREY;
	wxColour _clrFoldBkg = _clrBackground;
	wxColour _clrFoldFrg = wxColour(240,240,240);

	wxColour _clrDefinition = wxColour(149,112,149);
	wxColour _clrOperator = wxColour(236,95,102);
	wxColour _clrWord = wxColour(0,0,150);
	wxColour _clrString = wxColour(153,199,148);
	wxColour _clrNumber = wxColour(249,174,88);
	wxColour _clrComment = wxColour(166,172,185);
	wxColour _clrCharacter = wxColour(153,199,148);
	wxColour _clrEscape = wxColour(198, 149, 198);

	// Dark mode
	wxColour _clrDarkText = *wxWHITE;
	wxColour _clrDarkBackground = wxColour(48,56,65);
	wxColour _clrDarkMargin = wxColour(5,5,5);
	wxColour _clrDarkLineNum = *wxLIGHT_GREY;
	wxColour _clrDarkFoldBkg = _clrDarkBackground;
	wxColour _clrDarkFoldFrg = wxColour(68,76,85);

	wxColour _clrDarkDefinition = wxColour(149,112,149);
	wxColour _clrDarkOperator = wxColour(236,95,102);
	wxColour _clrDarkWord = *wxRED;
	wxColour _clrDarkString = wxColour(153,199,148);
	wxColour _clrDarkNumber = wxColour(249,174,88);
	wxColour _clrDarkComment = wxColour(166,172,185);
	wxColour _clrDarkCharacter = wxColour(153,199,148);
	wxColour _clrDarkEscape = wxColour(198, 149, 198);

	// Toolbar widgets
	wxButton *_saveFile = nullptr;
	wxSearchCtrl *_searchFile = nullptr;
	wxCheckBox *_wordWrap = nullptr;
	wxChoice *_darkMode = nullptr;

	// Map of file extensions and their code lexers
	std::unordered_map<wxString, int> _lexerType = {{"asm", wxSTC_LEX_ASM}, {"c", wxSTC_LEX_CPP},   {"cpp", wxSTC_LEX_CPP}, {"cs", wxSTC_LEX_CPP},
													{"cob", wxSTC_LEX_COBOL}, {"for", wxSTC_LEX_FORTRAN}, {"go", wxSTC_LEX_CPP},  {"h", wxSTC_LEX_CPP},
													{"hpp", wxSTC_LEX_CPP}, {"hs", wxSTC_LEX_HASKELL},  {"java", wxSTC_LEX_CPP}, {"lisp", wxSTC_LEX_LISP},
													{"m", wxSTC_LEX_CPP},   {"pas", wxSTC_LEX_PASCAL}, {"php", wxSTC_LEX_PHPSCRIPT}, {"html", wxSTC_LEX_HTML},
													{"py", wxSTC_LEX_PYTHON},  {"r", wxSTC_LEX_R},   {"rb", wxSTC_LEX_RUBY},  {"vb", wxSTC_LEX_VB}};

	std::unordered_map<wxColour, wxColour, ColourHash> _lightToDark = {{_clrDefinition, _clrDarkDefinition}, {_clrOperator, _clrDarkOperator}, {_clrWord, _clrDarkWord}, {_clrString, _clrDarkString},
														   			   {_clrNumber, _clrDarkNumber}, {_clrComment, _clrDarkComment}, {_clrCharacter, _clrDarkCharacter}, {_clrText, _clrDarkText}, {_clrEscape, _clrDarkEscape}};

	wxColour _paletteASM[17] = {_clrText, _clrComment, _clrNumber, _clrString, _clrOperator, _clrText, _clrNumber, _clrNumber, _clrOperator, _clrOperator, _clrOperator, _clrComment, _clrCharacter, _clrString, _clrNumber, _clrComment, wxNullColour};
	wxColour _paletteCPP[29] = {_clrText, _clrComment, _clrComment, _clrComment, _clrNumber, _clrWord, _clrString, _clrCharacter, _clrText, _clrOperator, _clrOperator, _clrText, _clrText, _clrText, _clrString, _clrComment, _clrWord, _clrComment, _clrComment, _clrOperator, _clrString, _clrText, _clrString, _clrComment, _clrComment, _clrText, _clrText, _clrEscape, wxNullColour};
	wxColour _palettePython[17] = {_clrText, _clrComment, _clrNumber, _clrString, _clrCharacter, _clrWord, _clrNumber, _clrNumber, _clrDefinition, _clrDefinition, _clrOperator, _clrText, _clrComment, _clrComment, _clrWord, _clrWord, wxNullColour};

	std::unordered_map<int, wxColour *> _paletteMap = {{wxSTC_LEX_ASM, _paletteASM}, {wxSTC_LEX_CPP, _paletteCPP}, {wxSTC_LEX_PYTHON, _palettePython}};

	TextView(wxWindow *parent, wxWindowID id, const wxPoint& pos,
			 const wxSize& size, long style, const wxString& name,
			 wxString filePath, wxButton *saveFile, wxSearchCtrl *searchFile,
			 wxCheckBox *wordWrap, wxChoice *darkMode);

	void setupTextView();
	void SyntaxHighlight(bool dark);
	void onTextFileSave(wxCommandEvent &event);
	void onMarginClick(wxStyledTextEvent &event);
	void onSearchFile(wxCommandEvent &event);
	void onWordWrap(wxCommandEvent &event);
	void onDarkMode(wxCommandEvent &event);
	void onFontChanged(wxFontPickerEvent& event);
};

#endif