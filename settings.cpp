#include "preview.h"

SettingsDialog::SettingsDialog(wxWindow *parent) : wxDialog(parent, wxID_ANY, "Settings", wxDefaultPosition, wxSize(350,500), wxCLOSE_BOX | wxSTAY_ON_TOP, wxEmptyString) {
	// First thing we need is a sizer for the whole dialog
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	// We also need a special sizer for the cancel/apply buttons at the bottom
	wxSizer *buttonSizer = CreateButtonSizer(wxCANCEL);

	// Which contains a cancel button by default, but we also want an apply button (we give it ID_OK so that it still closes the window and signals affirmative)
	wxButton *applyButton = new wxButton(this, wxID_OK, "Apply");

	// And we want the apply button to be bound to a function for saving the settings	
	applyButton->Bind(wxEVT_BUTTON, &SettingsDialog::onApply, this);
	buttonSizer->Add(applyButton, 0, wxALIGN_CENTER);

	// Next we want a notebook to separate the settings by file type
	wxNotebook *settingsBook = new wxNotebook(this, wxID_ANY);

	// Which has just two panels, one for general and one for text
	wxPanel *panelGeneral = new wxPanel(settingsBook, wxID_ANY);
	wxScrolledWindow *panelText = new wxScrolledWindow(settingsBook, wxID_ANY);
	panelText->SetScrollRate(5, 5);

	// Which we add as pages to the notebook
	settingsBook->AddPage(panelGeneral, "General");
	settingsBook->AddPage(panelText, "Text");

	// Now we can populate the general panel with controls
	wxFlexGridSizer *generalSizer = new wxFlexGridSizer(2, 10, 20);
	
	// We have a choice box for the size setting
	wxStaticText *sizeText = new wxStaticText(panelGeneral, wxID_ANY, "Window Size ");
	wxString sizeChoices[3] = {"Small", "Medium", "Large"};
	_sizeChoice = new wxChoice(panelGeneral, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, sizeChoices, 0, wxDefaultValidator, wxEmptyString);
	
	// And a choice box for the binary default open setting
	wxStaticText *binaryDefaultText = new wxStaticText(panelGeneral, wxID_ANY, "Binary Opens as");
	wxString binaryChoices[3] = {"Hex", "Palette", "Graphics"};
	_binaryChoice = new wxChoice(panelGeneral, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, binaryChoices, 0, wxDefaultValidator, wxEmptyString);

	// The bits per colour is a spin control
	wxStaticText *BPCDefaultText = new wxStaticText(panelGeneral, wxID_ANY, "Bits Per Colour Default");
	_BPCSpin = new wxSpinCtrl(panelGeneral, wxID_ANY, "5", wxDefaultPosition, wxDefaultSize, 0, 1, 8, 0, wxEmptyString);

	// Same for the bits per pixel default
	wxStaticText *BPPDefaultText = new wxStaticText(panelGeneral, wxID_ANY, "Bits Per Pixel ");
	_BPPSpin = new wxSpinCtrl(panelGeneral, wxID_ANY, "4", wxDefaultPosition, wxDefaultSize, 0, 1, 8, 0, wxEmptyString);

	// Add everything to the sizer
	generalSizer->Add(sizeText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxTOP, 5);
	generalSizer->Add(_sizeChoice, 0, wxGROW | wxTOP, 5);
	generalSizer->Add(binaryDefaultText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	generalSizer->Add(_binaryChoice, 0, wxGROW);
	generalSizer->Add(BPCDefaultText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	generalSizer->Add(_BPCSpin, 0, wxGROW);
	generalSizer->Add(BPPDefaultText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	generalSizer->Add(_BPPSpin, 0, wxGROW);

	// And set the sizer for the panel
	panelGeneral->SetSizer(generalSizer);

	// Now we can populate the text panel
	wxBoxSizer *textBoxSizer = new wxBoxSizer(wxVERTICAL);

	// First thing we want is a font picker
	wxBoxSizer *fontSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *fontText = new wxStaticText(panelText, wxID_ANY, "Font: ");
	wxFontPickerCtrl *fontCtrl = new wxFontPickerCtrl(panelText, wxID_ANY, wxNullFont, wxDefaultPosition, wxDefaultSize, wxFNTP_DEFAULT_STYLE, wxDefaultValidator, wxEmptyString);
	fontSizer->Add(fontText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	fontSizer->Add(fontCtrl, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	// And now we can add it to the sizer
	textBoxSizer->Add(fontSizer, 0, wxTOP, 10);

	// First up are the light mode colours
	wxStaticBoxSizer *textBox = new wxStaticBoxSizer(wxVERTICAL, panelText, "Light Mode");
	wxFlexGridSizer *textSizer = new wxFlexGridSizer(2, 5, 20);

	wxStaticText *lightModeMarginText = new wxStaticText(textBox->GetStaticBox(), wxID_ANY, "Margin Colour");
	_lightModeMargin = new wxColourPickerCtrl(textBox->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *lightModeLineNumText = new wxStaticText(textBox->GetStaticBox(), wxID_ANY, "Line Number Colour");
	_lightModeLineNum = new wxColourPickerCtrl(textBox->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *lightModeTextText = new wxStaticText(textBox->GetStaticBox(), wxID_ANY, "Text Colour");
	_lightModeText = new wxColourPickerCtrl(textBox->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *lightModeBackgroundText = new wxStaticText(textBox->GetStaticBox(), wxID_ANY, "Background Colour");
	_lightModeBackground = new wxColourPickerCtrl(textBox->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *lightModeDefinitionText = new wxStaticText(textBox->GetStaticBox(), wxID_ANY, "Definition Colour");
	_lightModeDefinition = new wxColourPickerCtrl(textBox->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *lightModeOperatorText = new wxStaticText(textBox->GetStaticBox(), wxID_ANY, "Operator Colour");
	_lightModeOperator = new wxColourPickerCtrl(textBox->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *lightModeWordText = new wxStaticText(textBox->GetStaticBox(), wxID_ANY, "Keyword Colour");
	_lightModeWord = new wxColourPickerCtrl(textBox->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *lightModeStringText = new wxStaticText(textBox->GetStaticBox(), wxID_ANY, "String Colour");
	_lightModeString = new wxColourPickerCtrl(textBox->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *lightModeNumberText = new wxStaticText(textBox->GetStaticBox(), wxID_ANY, "Number Colour");
	_lightModeNumber = new wxColourPickerCtrl(textBox->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *lightModeCommentText = new wxStaticText(textBox->GetStaticBox(), wxID_ANY, "Comment Colour");
	_lightModeComment = new wxColourPickerCtrl(textBox->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *lightModeCharacterText = new wxStaticText(textBox->GetStaticBox(), wxID_ANY, "Character Colour");
	_lightModeCharacter = new wxColourPickerCtrl(textBox->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *lightModeEscapeText = new wxStaticText(textBox->GetStaticBox(), wxID_ANY, "Escape Colour");
	_lightModeEscape = new wxColourPickerCtrl(textBox->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);

	// And now we add all of them to the text sizer
	textSizer->Add(lightModeMarginText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizer->Add(_lightModeMargin, 0, wxGROW);
	textSizer->Add(lightModeLineNumText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizer->Add(_lightModeLineNum, 0, wxGROW);
	textSizer->Add(lightModeTextText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizer->Add(_lightModeText, 0, wxGROW);
	textSizer->Add(lightModeBackgroundText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizer->Add(_lightModeBackground, 0, wxGROW);
	textSizer->Add(lightModeDefinitionText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizer->Add(_lightModeDefinition, 0, wxGROW);
	textSizer->Add(lightModeOperatorText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizer->Add(_lightModeOperator, 0, wxGROW);
	textSizer->Add(lightModeWordText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizer->Add(_lightModeWord, 0, wxGROW);
	textSizer->Add(lightModeStringText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizer->Add(_lightModeString, 0, wxGROW);
	textSizer->Add(lightModeNumberText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizer->Add(_lightModeNumber, 0, wxGROW);
	textSizer->Add(lightModeCommentText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizer->Add(_lightModeComment, 0, wxGROW);
	textSizer->Add(lightModeCharacterText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizer->Add(_lightModeCharacter, 0, wxGROW);
	textSizer->Add(lightModeEscapeText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizer->Add(_lightModeEscape, 0, wxGROW);

	textBox->Add(textSizer, 0, wxGROW);
	textBoxSizer->Add(textBox, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP, 10);

	// Next are the dark mode colours
	wxStaticBoxSizer *textBoxDark = new wxStaticBoxSizer(wxVERTICAL, panelText, "Dark Mode");
	wxFlexGridSizer *textSizerDark = new wxFlexGridSizer(2, 5, 20);

	wxStaticText *darkModeMarginText = new wxStaticText(textBoxDark->GetStaticBox(), wxID_ANY, "Margin Colour");
	_darkModeMargin = new wxColourPickerCtrl(textBoxDark->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *darkModeLineNumText = new wxStaticText(textBoxDark->GetStaticBox(), wxID_ANY, "Line Number Colour");
	_darkModeLineNum = new wxColourPickerCtrl(textBoxDark->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *darkModeTextText = new wxStaticText(textBoxDark->GetStaticBox(), wxID_ANY, "Text Colour");
	_darkModeText = new wxColourPickerCtrl(textBoxDark->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *darkModeBackgroundText = new wxStaticText(textBoxDark->GetStaticBox(), wxID_ANY, "Background Colour");
	_darkModeBackground = new wxColourPickerCtrl(textBoxDark->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *darkModeDefinitionText = new wxStaticText(textBoxDark->GetStaticBox(), wxID_ANY, "Definition Colour");
	_darkModeDefinition = new wxColourPickerCtrl(textBoxDark->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *darkModeOperatorText = new wxStaticText(textBoxDark->GetStaticBox(), wxID_ANY, "Operator Colour");
	_darkModeOperator = new wxColourPickerCtrl(textBoxDark->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *darkModeWordText = new wxStaticText(textBoxDark->GetStaticBox(), wxID_ANY, "Keyword Colour");
	_darkModeWord = new wxColourPickerCtrl(textBoxDark->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *darkModeStringText = new wxStaticText(textBoxDark->GetStaticBox(), wxID_ANY, "String Colour");
	_darkModeString = new wxColourPickerCtrl(textBoxDark->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *darkModeNumberText = new wxStaticText(textBoxDark->GetStaticBox(), wxID_ANY, "Number Colour");
	_darkModeNumber = new wxColourPickerCtrl(textBoxDark->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *darkModeCommentText = new wxStaticText(textBoxDark->GetStaticBox(), wxID_ANY, "Comment Colour");
	_darkModeComment = new wxColourPickerCtrl(textBoxDark->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *darkModeCharacterText = new wxStaticText(textBoxDark->GetStaticBox(), wxID_ANY, "Character Colour");
	_darkModeCharacter = new wxColourPickerCtrl(textBoxDark->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);
	wxStaticText *darkModeEscapeText = new wxStaticText(textBoxDark->GetStaticBox(), wxID_ANY, "Escape Colour");
	_darkModeEscape = new wxColourPickerCtrl(textBoxDark->GetStaticBox(), wxID_ANY, wxColour(*wxBLACK), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);

	// And now we add all of them to the text sizer
	textSizerDark->Add(darkModeMarginText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizerDark->Add(_darkModeMargin, 0, wxGROW);
	textSizerDark->Add(darkModeLineNumText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizerDark->Add(_darkModeLineNum, 0, wxGROW);
	textSizerDark->Add(darkModeTextText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizerDark->Add(_darkModeText, 0, wxGROW);
	textSizerDark->Add(darkModeBackgroundText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizerDark->Add(_darkModeBackground, 0, wxGROW);
	textSizerDark->Add(darkModeDefinitionText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizerDark->Add(_darkModeDefinition, 0, wxGROW);
	textSizerDark->Add(darkModeOperatorText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizerDark->Add(_darkModeOperator, 0, wxGROW);
	textSizerDark->Add(darkModeWordText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizerDark->Add(_darkModeWord, 0, wxGROW);
	textSizerDark->Add(darkModeStringText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizerDark->Add(_darkModeString, 0, wxGROW);
	textSizerDark->Add(darkModeNumberText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizerDark->Add(_darkModeNumber, 0, wxGROW);
	textSizerDark->Add(darkModeCommentText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizerDark->Add(_darkModeComment, 0, wxGROW);
	textSizerDark->Add(darkModeCharacterText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizerDark->Add(_darkModeCharacter, 0, wxGROW);
	textSizerDark->Add(darkModeEscapeText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	textSizerDark->Add(_darkModeEscape, 0, wxGROW);

	textBoxDark->Add(textSizerDark, 0, wxGROW);
	textBoxSizer->Add(textBoxDark, 0, wxGROW | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);

	panelText->SetSizer(textBoxSizer);

	// Now we make sure the button sizer is at the bottom with a stretch spacer
	mainSizer->Add(settingsBook, 1, wxGROW | wxLEFT | wxTOP | wxRIGHT, kMacMargins);
	mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP, 5);

	// And finally set the main sizer to be the sizer for this dialog
	SetSizer(mainSizer);
}

void SettingsDialog::onApply(wxCommandEvent &event) {

}





