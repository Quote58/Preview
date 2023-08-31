// Preview

#ifndef PREVIEW_H
#define PREVIEW_H

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include <wx/notebook.h>
#include "wx/colordlg.h"
#include "wx/fontdlg.h"
#include "wx/numdlg.h"
#include "wx/aboutdlg.h"
#include <wx/richtext/richtextctrl.h>
#include <wx/collpane.h>
#include <wx/statline.h>
#include <wx/filectrl.h>
#include <wx/dcbuffer.h>
#include <wx/preferences.h>
#include <unordered_map>
#include <wx/fontpicker.h>

#include <wx/dirctrl.h>
#include <wx/stdpaths.h>
#include "wx/grid.h"
#include "wx/headerctrl.h"
#include "wx/generic/gridctrl.h"
#include "wx/generic/grideditors.h"
#include <wx/tglbtn.h>
#include <wx/toolbar.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/artprov.h>
#include <wx/vector.h>
#include <wx/checkbox.h>
#include <wx/log.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/srchctrl.h>
#include <wx/statbox.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/scrolwin.h>
#include <wx/clrpicker.h>
#include <wx/windowptr.h>
#include <wx/stc/stc.h>
#include "wx/numformatter.h"
#include "wx/renderer.h"
#include "wx/uilocale.h"
#include <wx/spinctrl.h>

#include "file.h"
#include "paletteView.h"
#include "textView.h"
#include "imageView.h"
#include "hexView.h"
#include "gfxView.h"

// For some reason this isn't a default template?
template<class T> using wxVector2D = wxVector<wxVector<T>>;

// Common margin values
enum CommonValues {
	kMacMargins = 19
};

// File extensions
enum Ext {
	EXT_text,
	EXT_code,
	EXT_asm,
	EXT_c,
	EXT_cpp,
	EXT_cs,
	EXT_cob,
	EXT_for,
	EXT_go,
	EXT_head,
	EXT_java,
	EXT_lisp,
	EXT_pas,
	EXT_php,
	EXT_html,
	EXT_py,
	EXT_r,
	EXT_ruby,
	EXT_vb,
	EXT_exe,
	EXT_snes,
	EXT_bin,
	EXT_pal,
	EXT_gfx,
	EXT_img,
	EXT_patch,
	EXT_bit,
	EXT_object,
	EXT_null
};

enum ID {
	// You can not add a menu ID of 0 on macOS (it's reserved for 'help' I think?)
	ID_Default,
	ID_STC,
	ID_ShowHide,
	ID_Settings
};

/* Preview Application
 * This class controls the entire program
 */
class Preview : public wxApp {
public:
	virtual bool OnInit();
};

/* Preview Main Frame (ha)
 * This class is the frame within which all
 * panels and controls get placed
 */
class PreviewFrame : public wxFrame {
public:
	PreviewFrame(wxSize s);

protected:
	/* For the toolbar */
	wxBitmapBundle _imageSidebar;
	wxBitmapBundle _imageSettings;

	/* Top level aspects of the program */

	// This map connects file types to distinct types so the program knows what to do with them
	std::unordered_map<wxString, Ext> _fileExt = {{"txt", EXT_text}, {"asm", EXT_asm}, {"c", EXT_c},   {"cpp", EXT_cpp}, {"cs", EXT_cs},  {"cob", EXT_cob}, {"for", EXT_for},
												  {"go", EXT_go}, {"h", EXT_head},   {"hpp", EXT_head}, {"hs", EXT_code},  {"java", EXT_java},
												  {"lisp", EXT_lisp}, {"m", EXT_code}, {"pas", EXT_pas}, {"php", EXT_php}, {"html", EXT_html}, {"py", EXT_py},  {"r", EXT_r},
												  {"rb", EXT_ruby}, {"vb", EXT_vb},
												  {"exe", EXT_exe},
												  {"bin", EXT_bin},
												  {"act", EXT_pal}, {"pal", EXT_pal}, {"tpl", EXT_pal}, {"gpl", EXT_pal},
												  {"gfx", EXT_gfx},
												  {"bmp", EXT_img}, {"png", EXT_img}, {"jpg", EXT_img}, {"gif", EXT_img}, {"xpm", EXT_img}, {"svg", EXT_img},
												  {"ips", EXT_patch}, {"bps", EXT_patch},
												  {"bit", EXT_bit},
												  {"o", EXT_object},
												  {"smc", EXT_snes}};

	// And then the types are connected further to strings, so that the program knows what to call these types of files
	std::unordered_map<Ext, wxString> _fileExtString = {{EXT_text, "Text"}, {EXT_exe, "Executable"}, {EXT_bin, "Binary"}, {EXT_pal, "Palette"},
														{EXT_gfx, "Graphics (raw)"}, {EXT_img, "Image"}, {EXT_patch, "Patch"}, {EXT_bit, "Bitmap"},
														{EXT_object, "Object"}, {EXT_null, "Invalid Type"},
														{EXT_snes, "Super Nintendo Rom"},
														{EXT_code, "Code"}, {EXT_asm, "Assembly Source"}, {EXT_c, "C source"}, {EXT_cpp, "C++ source"},
														{EXT_cs, "C Sharp source"}, {EXT_cob, "Cobalt Source"}, {EXT_for, "Fortran source"}, {EXT_go, "Go source"},
														{EXT_head, "Header"}, {EXT_java, "Java Source"}, {EXT_lisp, "Lisp source"}, {EXT_pas, "Pascal source"},
														{EXT_php, "PHP source"}, {EXT_html, "HTML"}, {EXT_py, "Python source"}, {EXT_r, "R source"},
														{EXT_ruby, "Ruby source"}, {EXT_vb, "Visual Basic Source"}};

	// _programSizer is a vertical box sizer that contains the currently visible panel
	// This is important in case I want more than one view of the program
	 wxToolBar *_toolBar;
	wxBoxSizer *_programSizer;

	// View Panel
	   wxPanel *_viewPanel;
	wxBoxSizer *_viewSizer;
	wxBoxSizer *_viewSideSizer;
	wxNotebook *_viewBook;

	// Sidebar
	wxStaticBoxSizer *_viewDirBox;
	wxGenericDirCtrl *_viewDir;
	wxStaticText *_viewFileName;
	wxStaticText *_viewFileType;
	wxStaticText *_viewFileSize;

	// Individual panels on the notebook
	wxToolBar  *_viewToolbar;

private:
	// Debug
	void debug(wxString s);
	void debug(int i);

	// Init functions
	void populateToolBar();
	void populateViewPanel();

	// Toolbar functions
	void onOpenSettings(wxCommandEvent &event);
	void onShowHideDir(wxCommandEvent &event);

	// ViewPanel functions
	void onViewDirFileChanged(wxCommandEvent &event);
	void onViewDirFileChosen(wxCommandEvent &event);
	void onCloseFile(wxCommandEvent &event);

	// File opening functions
	void openExecutableFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer);
	void openTextFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer);
	void openBinaryFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer);
	void openImageFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer);
	void openPatchFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer);
	void openBitsFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer);
	void openPaletteFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer);
	void onPaletteViewCopyPaste(wxKeyEvent &event);
	void onPaletteViewDClick(wxGridEvent& event);
	void onPaletteViewClick(wxGridEvent &event);
	void onPaletteViewClickMultiple(wxGridRangeSelectEvent &event);
	void openGfxFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer);
	void onGfxViewClick(wxGridEvent &event);
	void onGfxViewClickMultiple(wxGridRangeSelectEvent &event);
	void onGfxPaletteTableClick(wxGridEvent &event);
	void onGfxPaletteTableDClick(wxGridEvent &event);
	void onGfxViewCopyPaste(wxKeyEvent &event);
	
	// General program functions

};

class SettingsDialog : public wxDialog {
public:
	SettingsDialog(wxWindow *parent);
	~SettingsDialog() {}

	// General controls
	wxChoice *_sizeChoice;
	wxChoice *_binaryChoice;
	wxSpinCtrl *_BPCSpin;
	wxSpinCtrl *_BPPSpin;

	// Text controls
	// Light mode
	wxColourPickerCtrl *_lightModeMargin;
	wxColourPickerCtrl *_lightModeLineNum;
	wxColourPickerCtrl *_lightModeText;
	wxColourPickerCtrl *_lightModeBackground;
	wxColourPickerCtrl *_lightModeDefinition;
	wxColourPickerCtrl *_lightModeOperator;
	wxColourPickerCtrl *_lightModeWord;
	wxColourPickerCtrl *_lightModeString;
	wxColourPickerCtrl *_lightModeNumber;
	wxColourPickerCtrl *_lightModeComment;
	wxColourPickerCtrl *_lightModeCharacter;
	wxColourPickerCtrl *_lightModeEscape;

	// Dark mode
	wxColourPickerCtrl *_darkModeMargin;
	wxColourPickerCtrl *_darkModeLineNum;
	wxColourPickerCtrl *_darkModeText;
	wxColourPickerCtrl *_darkModeBackground;
	wxColourPickerCtrl *_darkModeDefinition;
	wxColourPickerCtrl *_darkModeOperator;
	wxColourPickerCtrl *_darkModeWord;
	wxColourPickerCtrl *_darkModeString;
	wxColourPickerCtrl *_darkModeNumber;
	wxColourPickerCtrl *_darkModeComment;
	wxColourPickerCtrl *_darkModeCharacter;
	wxColourPickerCtrl *_darkModeEscape;

	void onApply(wxCommandEvent &event);
};

DECLARE_APP(Preview)

#endif



























