// Preview
#include "preview.h"
#include "file.h"

IMPLEMENT_APP(Preview)

// App methods
bool Preview::OnInit() {
	// Preview can use all kinds of image files, so init all the handlers
	wxInitAllImageHandlers();

	// Which is by default 800x600
	int sizeX = 1080;
	int sizeY = 720;

	// Create the frame that will contain the program
	PreviewFrame *frame = new PreviewFrame(wxSize(sizeX, sizeY));
	frame->Show(true);

	// Hexer needs to be resizable, but we don't want it going smaller than the default
	frame->SetMinSize(wxSize(sizeX, sizeY));

	return true;
}

// ------------------------------------------------------------------
/* ****               ****
 * **** The App Frame ****
 * ****               ****
 */
void PreviewFrame::debug(wxString s) {
	std::cout << s << std::endl;
}

void PreviewFrame::debug(int i) {
	std::cout << i << std::endl;
}

PreviewFrame::PreviewFrame(wxSize s) : wxFrame(NULL, wxID_ANY, "Preview", wxDefaultPosition, s) {
	// The images need to be loaded in from their SVG strings
	if (wxSystemSettings::GetAppearance().IsDark()) {
		_imageSidebar = wxBitmapBundle::FromSVG("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!--Generator: Apple Native CoreSVG 232.5-->\n<!DOCTYPE svg\nPUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n       \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"29.175\" height=\"22.2332\">\n <g>\n  <rect height=\"22.2332\" opacity=\"0\" width=\"29.175\" x=\"0\" y=\"0\"/>\n  <path d=\"M3.32072 22.2332L25.8516 22.2332C28.0293 22.2332 29.175 21.0621 29.175 18.9164L29.175 3.3672C29.175 1.22149 28.0293 0.0503898 25.8516 0.0503898L3.32072 0.0503898C1.14571 0.0503898 0 1.19063 0 3.3672L0 18.9164C0 21.086 1.14571 22.2332 3.32072 22.2332ZM3.35275 21.4469C1.66214 21.4469 0.786366 20.5695 0.786366 18.8777L0.786366 3.40588C0.786366 1.71409 1.66214 0.836756 3.35275 0.836756L25.8223 0.836756C27.4484 0.836756 28.3859 1.71409 28.3859 3.40588L28.3859 18.8777C28.3859 20.5695 27.4484 21.4469 25.8223 21.4469ZM9.01017 21.6129L9.79263 21.6129L9.79263 0.52191L9.01017 0.52191ZM6.61213 5.55941C6.82737 5.55941 6.99378 5.39026 6.99378 5.19456C6.99378 4.99338 6.82737 4.83673 6.61213 4.83673L3.19262 4.83673C2.97738 4.83673 2.818 4.99338 2.818 5.19456C2.818 5.39026 2.97738 5.55941 3.19262 5.55941ZM6.61213 9.00784C6.82737 9.00784 6.99378 8.84143 6.99378 8.65002C6.99378 8.45158 6.82737 8.2879 6.61213 8.2879L3.19262 8.2879C2.97738 8.2879 2.818 8.45158 2.818 8.65002C2.818 8.84143 2.97738 9.00784 3.19262 9.00784ZM6.61213 12.4422C6.82737 12.4422 6.99378 12.2856 6.99378 12.0871C6.99378 11.8887 6.82737 11.725 6.61213 11.725L3.19262 11.725C2.97738 11.725 2.818 11.8887 2.818 12.0871C2.818 12.2856 2.97738 12.4422 3.19262 12.4422Z\" fill=\"#ffffff\" fill-opacity=\"0.85\"/>\n </g>\n</svg>\n", wxSize(29,22));
		_imageSettings = wxBitmapBundle::FromSVG("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!--Generator: Apple Native CoreSVG 232.5-->\n<!DOCTYPE svg\nPUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n       \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"25.8317\" height=\"25.8403\">\n <g>\n  <rect height=\"25.8403\" opacity=\"0\" width=\"25.8317\" x=\"0\" y=\"0\"/>\n  <path d=\"M11.6414 25.8403L14.1903 25.8403C14.9395 25.8403 15.4332 25.434 15.6129 24.6481L16.2047 22.1051L17.1258 21.7649L19.3422 23.1594C20.0039 23.5871 20.6508 23.5219 21.1856 22.9871L22.9645 21.2055C23.509 20.6707 23.584 20.0238 23.1563 19.3508L21.7563 17.1485L22.0965 16.2188L24.6324 15.627C25.4227 15.4473 25.8317 14.9508 25.8317 14.1945L25.8317 11.6594C25.8317 10.9129 25.4254 10.4164 24.6324 10.2367L22.102 9.64649L21.752 8.70432L23.159 6.50471C23.5868 5.82893 23.5118 5.18751 22.9672 4.6543L21.1813 2.87266C20.6492 2.34063 20.0024 2.26562 19.3406 2.69063L17.1242 4.09219L16.2047 3.74649L15.6129 1.20626C15.4332 0.417583 14.9395 0.0140619 14.1903 0.0140619L11.6414 0.0140619C10.8922 0.0140619 10.3984 0.417583 10.2188 1.20626L9.62696 3.74649L8.69064 4.09219L6.47698 2.69063C5.81525 2.26562 5.16837 2.34063 4.63633 2.87266L2.85469 4.6543C2.30586 5.18751 2.23086 5.82893 2.6586 6.50471L4.06562 8.70432L3.71289 9.64649L1.19923 10.2367C0.406256 10.4164 0 10.9129 0 11.6594L0 14.1945C0 14.9508 0.40899 15.4473 1.19923 15.627L3.71836 16.2188L4.06836 17.1485L2.66133 19.3508C2.23359 20.0238 2.3086 20.6707 2.8504 21.2055L4.63907 22.9871C5.16681 23.5219 5.81369 23.5871 6.47542 23.1594L8.68908 21.7649L9.62696 22.1051L10.2188 24.6481C10.3984 25.434 10.8922 25.8403 11.6414 25.8403ZM11.7539 25.0605C11.3145 25.0605 11.0555 24.8352 10.9645 24.4293L10.2953 21.4488C9.74143 21.2816 9.1633 21.0715 8.68517 20.8574L6.12345 22.4809C5.76369 22.716 5.41799 22.6727 5.09612 22.3508L3.46996 20.7316C3.15512 20.4125 3.12426 20.0934 3.36644 19.7141L4.98012 17.1383C4.79378 16.6797 4.56136 16.1016 4.39417 15.5477L1.41799 14.8813C1.00511 14.7902 0.779724 14.5313 0.779724 14.0891L0.779724 11.7692C0.779724 11.327 1.00238 11.0707 1.41799 10.977L4.39144 10.3078C4.55863 9.75276 4.79496 9.16916 4.98168 8.71447L3.36371 6.14142C3.11879 5.76643 3.14965 5.44456 3.47152 5.12542L5.09339 3.50199C5.41526 3.18285 5.76252 3.13949 6.12345 3.38168L8.68674 4.99691C9.15783 4.79105 9.73987 4.56566 10.2981 4.4055L10.9645 1.42503C11.0555 1.01644 11.3145 0.793786 11.7539 0.793786L14.0777 0.793786C14.534 0.793786 14.7762 0.985579 14.8672 1.42503L15.5363 4.40394C16.1238 4.56839 16.6727 4.79378 17.1324 4.99965L19.6914 3.38168C20.0621 3.13949 20.3996 3.18285 20.7242 3.50199L22.3461 5.12542C22.6652 5.44456 22.6961 5.76643 22.4512 6.14142L20.8402 8.71447C21.0535 9.16916 21.2731 9.73596 21.4402 10.3078L24.4137 10.977C24.8293 11.0707 25.052 11.327 25.052 11.7692L25.052 14.0891C25.052 14.5313 24.8266 14.7902 24.4137 14.8813L21.4375 15.5477C21.2703 16.1156 21.0547 16.6797 20.8348 17.1383L22.4484 19.7141C22.6906 20.0934 22.6668 20.4125 22.3477 20.7316L20.7215 22.3508C20.4039 22.6727 20.0539 22.716 19.6914 22.4809L17.1297 20.8574C16.6684 21.0715 16.1211 21.2816 15.5363 21.4488L14.8672 24.4293C14.7762 24.866 14.534 25.0605 14.0777 25.0605ZM12.9278 17.7508C15.5645 17.7508 17.7535 15.5617 17.7535 12.9153C17.7535 10.2883 15.5645 8.10627 12.9278 8.10627C10.291 8.10627 8.08947 10.2883 8.08947 12.9153C8.08947 15.566 10.2742 17.7508 12.9278 17.7508ZM12.9278 16.9738C10.7239 16.9738 8.87193 15.1219 8.87193 12.9153C8.87193 10.7309 10.7336 8.88326 12.9278 8.88326C15.1262 8.88326 16.9711 10.7309 16.9711 12.9153C16.9711 15.1191 15.1262 16.9738 12.9278 16.9738Z\" fill=\"#ffffff\" fill-opacity=\"0.85\"/>\n </g>\n</svg>\n", wxSize(26,26));
	} else {
		_imageSidebar = wxBitmapBundle::FromSVG("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!--Generator: Apple Native CoreSVG 232.5-->\n<!DOCTYPE svg\nPUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n       \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"29.175\" height=\"22.2332\">\n <g>\n  <rect height=\"22.2332\" opacity=\"0\" width=\"29.175\" x=\"0\" y=\"0\"/>\n  <path d=\"M3.32072 22.2332L25.8516 22.2332C28.0293 22.2332 29.175 21.0621 29.175 18.9164L29.175 3.3672C29.175 1.22149 28.0293 0.0503898 25.8516 0.0503898L3.32072 0.0503898C1.14571 0.0503898 0 1.19063 0 3.3672L0 18.9164C0 21.086 1.14571 22.2332 3.32072 22.2332ZM3.35275 21.4469C1.66214 21.4469 0.786366 20.5695 0.786366 18.8777L0.786366 3.40588C0.786366 1.71409 1.66214 0.836756 3.35275 0.836756L25.8223 0.836756C27.4484 0.836756 28.3859 1.71409 28.3859 3.40588L28.3859 18.8777C28.3859 20.5695 27.4484 21.4469 25.8223 21.4469ZM9.01017 21.6129L9.79263 21.6129L9.79263 0.52191L9.01017 0.52191ZM6.61213 5.55941C6.82737 5.55941 6.99378 5.39026 6.99378 5.19456C6.99378 4.99338 6.82737 4.83673 6.61213 4.83673L3.19262 4.83673C2.97738 4.83673 2.818 4.99338 2.818 5.19456C2.818 5.39026 2.97738 5.55941 3.19262 5.55941ZM6.61213 9.00784C6.82737 9.00784 6.99378 8.84143 6.99378 8.65002C6.99378 8.45158 6.82737 8.2879 6.61213 8.2879L3.19262 8.2879C2.97738 8.2879 2.818 8.45158 2.818 8.65002C2.818 8.84143 2.97738 9.00784 3.19262 9.00784ZM6.61213 12.4422C6.82737 12.4422 6.99378 12.2856 6.99378 12.0871C6.99378 11.8887 6.82737 11.725 6.61213 11.725L3.19262 11.725C2.97738 11.725 2.818 11.8887 2.818 12.0871C2.818 12.2856 2.97738 12.4422 3.19262 12.4422Z\" fill=\"#000000\" fill-opacity=\"0.85\"/>\n </g>\n</svg>\n", wxSize(29,22));
		_imageSettings = wxBitmapBundle::FromSVG("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!--Generator: Apple Native CoreSVG 232.5-->\n<!DOCTYPE svg\nPUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n       \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"25.8317\" height=\"25.8403\">\n <g>\n  <rect height=\"25.8403\" opacity=\"0\" width=\"25.8317\" x=\"0\" y=\"0\"/>\n  <path d=\"M11.6414 25.8403L14.1903 25.8403C14.9395 25.8403 15.4332 25.434 15.6129 24.6481L16.2047 22.1051L17.1258 21.7649L19.3422 23.1594C20.0039 23.5871 20.6508 23.5219 21.1856 22.9871L22.9645 21.2055C23.509 20.6707 23.584 20.0238 23.1563 19.3508L21.7563 17.1485L22.0965 16.2188L24.6324 15.627C25.4227 15.4473 25.8317 14.9508 25.8317 14.1945L25.8317 11.6594C25.8317 10.9129 25.4254 10.4164 24.6324 10.2367L22.102 9.64649L21.752 8.70432L23.159 6.50471C23.5868 5.82893 23.5118 5.18751 22.9672 4.6543L21.1813 2.87266C20.6492 2.34063 20.0024 2.26562 19.3406 2.69063L17.1242 4.09219L16.2047 3.74649L15.6129 1.20626C15.4332 0.417583 14.9395 0.0140619 14.1903 0.0140619L11.6414 0.0140619C10.8922 0.0140619 10.3984 0.417583 10.2188 1.20626L9.62696 3.74649L8.69064 4.09219L6.47698 2.69063C5.81525 2.26562 5.16837 2.34063 4.63633 2.87266L2.85469 4.6543C2.30586 5.18751 2.23086 5.82893 2.6586 6.50471L4.06562 8.70432L3.71289 9.64649L1.19923 10.2367C0.406256 10.4164 0 10.9129 0 11.6594L0 14.1945C0 14.9508 0.40899 15.4473 1.19923 15.627L3.71836 16.2188L4.06836 17.1485L2.66133 19.3508C2.23359 20.0238 2.3086 20.6707 2.8504 21.2055L4.63907 22.9871C5.16681 23.5219 5.81369 23.5871 6.47542 23.1594L8.68908 21.7649L9.62696 22.1051L10.2188 24.6481C10.3984 25.434 10.8922 25.8403 11.6414 25.8403ZM11.7539 25.0605C11.3145 25.0605 11.0555 24.8352 10.9645 24.4293L10.2953 21.4488C9.74143 21.2816 9.1633 21.0715 8.68517 20.8574L6.12345 22.4809C5.76369 22.716 5.41799 22.6727 5.09612 22.3508L3.46996 20.7316C3.15512 20.4125 3.12426 20.0934 3.36644 19.7141L4.98012 17.1383C4.79378 16.6797 4.56136 16.1016 4.39417 15.5477L1.41799 14.8813C1.00511 14.7902 0.779724 14.5313 0.779724 14.0891L0.779724 11.7692C0.779724 11.327 1.00238 11.0707 1.41799 10.977L4.39144 10.3078C4.55863 9.75276 4.79496 9.16916 4.98168 8.71447L3.36371 6.14142C3.11879 5.76643 3.14965 5.44456 3.47152 5.12542L5.09339 3.50199C5.41526 3.18285 5.76252 3.13949 6.12345 3.38168L8.68674 4.99691C9.15783 4.79105 9.73987 4.56566 10.2981 4.4055L10.9645 1.42503C11.0555 1.01644 11.3145 0.793786 11.7539 0.793786L14.0777 0.793786C14.534 0.793786 14.7762 0.985579 14.8672 1.42503L15.5363 4.40394C16.1238 4.56839 16.6727 4.79378 17.1324 4.99965L19.6914 3.38168C20.0621 3.13949 20.3996 3.18285 20.7242 3.50199L22.3461 5.12542C22.6652 5.44456 22.6961 5.76643 22.4512 6.14142L20.8402 8.71447C21.0535 9.16916 21.2731 9.73596 21.4402 10.3078L24.4137 10.977C24.8293 11.0707 25.052 11.327 25.052 11.7692L25.052 14.0891C25.052 14.5313 24.8266 14.7902 24.4137 14.8813L21.4375 15.5477C21.2703 16.1156 21.0547 16.6797 20.8348 17.1383L22.4484 19.7141C22.6906 20.0934 22.6668 20.4125 22.3477 20.7316L20.7215 22.3508C20.4039 22.6727 20.0539 22.716 19.6914 22.4809L17.1297 20.8574C16.6684 21.0715 16.1211 21.2816 15.5363 21.4488L14.8672 24.4293C14.7762 24.866 14.534 25.0605 14.0777 25.0605ZM12.9278 17.7508C15.5645 17.7508 17.7535 15.5617 17.7535 12.9153C17.7535 10.2883 15.5645 8.10627 12.9278 8.10627C10.291 8.10627 8.08947 10.2883 8.08947 12.9153C8.08947 15.566 10.2742 17.7508 12.9278 17.7508ZM12.9278 16.9738C10.7239 16.9738 8.87193 15.1219 8.87193 12.9153C8.87193 10.7309 10.7336 8.88326 12.9278 8.88326C15.1262 8.88326 16.9711 10.7309 16.9711 12.9153C16.9711 15.1191 15.1262 16.9738 12.9278 16.9738Z\" fill=\"#000000\" fill-opacity=\"0.85\"/>\n </g>\n</svg>", wxSize(26,26));
	}

	// First we give it a toolbar (this will probably be removed on windows)
	_toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_TEXT, wxEmptyString);
	populateToolBar();
	SetToolBar(_toolBar);
	_toolBar->Realize();

	// After the toolbar is the main program area, which needs a general sizer to correctly size the panel when resizing
	_programSizer = new wxBoxSizer(wxVERTICAL);
	   _viewPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, "Main Panel");
	populateViewPanel();
	_programSizer->Add(_viewPanel, 1, wxGROW);

	// And now we need to apply the sizer to the frame
	_programSizer->Layout();
	SetSizer(_programSizer);
}
// ------------------------------------------------------------------

// ------------------------------------------------------------------
/* ****              ****
 * **** The Tool Bar ****
 * ****              ****
 */
void PreviewFrame::populateToolBar() {
	// First we have the settings button
	wxButton *settings = new wxButton(_toolBar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxBU_EXACTFIT, wxDefaultValidator, wxEmptyString);
	wxToolBarToolBase *settingsTool = _toolBar->CreateTool(ID_Settings, "Settings", _imageSettings, wxNullBitmap, wxITEM_NORMAL, settings, "Open Settings", wxEmptyString);

	// And then the show or hide the directory button
	wxButton *showHideDir = new wxButton(_toolBar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxBU_EXACTFIT, wxDefaultValidator, wxEmptyString);
	wxToolBarToolBase *showHideDirTool = _toolBar->CreateTool(ID_ShowHide, "Hide", _imageSidebar, wxNullBitmap, wxITEM_NORMAL, showHideDir, "Show or Hide the Directory", wxEmptyString);
	
	// We add them to the toolbar
	_toolBar->AddTool(showHideDirTool);
	_toolBar->AddTool(settingsTool);

	// And then bind them to functions
	_toolBar->Bind(wxEVT_TOOL, &PreviewFrame::onOpenSettings, this, ID_Settings);
	_toolBar->Bind(wxEVT_TOOL, &PreviewFrame::onShowHideDir, this, ID_ShowHide);
}
// ------------------------------------------------------------------

void PreviewFrame::onOpenSettings(wxCommandEvent &event) {
	SettingsDialog *settings = new SettingsDialog(this);
	settings->ShowWindowModal();
}

// And this function just does the actual showing and hiding of the directory panel
void PreviewFrame::onShowHideDir(wxCommandEvent &event) {
	wxToolBarToolBase *button = _toolBar->FindById(ID_ShowHide);

	// If the directory is currently shown, change the button to 'show' and hide the directory
	if (_viewDirBox->GetStaticBox()->IsShown()) {
		button->SetLabel("Show");
		_viewDirBox->GetStaticBox()->Hide();

	// If it is not shown, change the button to 'hide' and show the directory
	} else {
		button->SetLabel("Hide");
		_viewDirBox->GetStaticBox()->Show(true);
	}

	// Now we need to send a size event to make sure all the sizers re-size themselves
	SendSizeEvent();
}

// ------------------------------------------------------------------
/* ****               ****
 * **** The Main View ****
 * ****               ****
 */
void PreviewFrame::populateViewPanel() {
	// We need an overall sizer for the horizontal elements
	_viewSizer = new wxBoxSizer(wxHORIZONTAL);

	// And a separate vertical sizer for the side panel	
	_viewSideSizer = new wxBoxSizer(wxVERTICAL);

	// Now we want to make a static box for the directory + file info
	_viewDirBox = new wxStaticBoxSizer(wxVERTICAL, _viewPanel, wxEmptyString);

	// First we want a static text of the word directory with a line under it
	wxStaticText *filesText = new wxStaticText(_viewDirBox->GetStaticBox(), wxID_ANY, "Directory");
	_viewDirBox->Add(filesText, 0, wxGROW);
	_viewDirBox->Add(new wxStaticLine(_viewDirBox->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize(0,12), wxLI_HORIZONTAL, wxEmptyString), 0, wxGROW);

	// And we need the file directory itself
	_viewDir = new wxGenericDirCtrl(_viewDirBox->GetStaticBox(), wxID_ANY, wxStandardPaths::Get().GetExecutablePath(), wxDefaultPosition, wxDefaultSize, 0, "", 0, "directory");
	_viewDir->GetTreeCtrl()->SetBackgroundColour(wxColour(0,0,0,0));
	_viewDir->SetBackgroundColour(wxColour(0,0,0,0));
	_viewDir->Bind(wxEVT_DIRCTRL_SELECTIONCHANGED, &PreviewFrame::onViewDirFileChanged, this);
	_viewDir->Bind(wxEVT_DIRCTRL_FILEACTIVATED, &PreviewFrame::onViewDirFileChosen, this);
	_viewDirBox->Add(_viewDir, 1, wxGROW | wxTOP, 15);

	// The file info section needs it's own sizers
	wxBoxSizer *fileNameSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *fileTypeSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *fileSizeSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *fileInfoName = new wxStaticText(_viewDirBox->GetStaticBox(), wxID_ANY, "Name");
				 _viewFileName = new wxStaticText(_viewDirBox->GetStaticBox(), wxID_ANY, "");
	wxStaticText *fileInfoType = new wxStaticText(_viewDirBox->GetStaticBox(), wxID_ANY, "Type");
				 _viewFileType = new wxStaticText(_viewDirBox->GetStaticBox(), wxID_ANY, "");
	wxStaticText *fileInfoSize = new wxStaticText(_viewDirBox->GetStaticBox(), wxID_ANY, "Size (bytes)");
				 _viewFileSize = new wxStaticText(_viewDirBox->GetStaticBox(), wxID_ANY, "");

	fileNameSizer->Add(fileInfoName, 0, wxALIGN_CENTER_VERTICAL);
	fileNameSizer->AddStretchSpacer();
	fileNameSizer->Add(_viewFileName, 0, wxALIGN_CENTER_VERTICAL);
	fileTypeSizer->Add(fileInfoType, 0, wxALIGN_CENTER_VERTICAL);
	fileTypeSizer->AddStretchSpacer();
	fileTypeSizer->Add(_viewFileType, 0, wxALIGN_CENTER_VERTICAL);
	fileSizeSizer->Add(fileInfoSize, 0, wxALIGN_CENTER_VERTICAL);
	fileSizeSizer->AddStretchSpacer();
	fileSizeSizer->Add(_viewFileSize, 0, wxALIGN_CENTER_VERTICAL);

	_viewDirBox->Add(fileNameSizer, 0, wxTOP | wxGROW, 5);
	_viewDirBox->Add(fileTypeSizer, 0, wxTOP | wxGROW, 5);
	_viewDirBox->Add(fileSizeSizer, 0, wxTOP | wxGROW, 5);
	
	// Now we can now add everything to the side panel sizer
	_viewSideSizer->Add(_viewDirBox, 1, wxGROW | wxTOP, 10);

	// Finally we have the notebook that will contain the file tabs
	_viewBook = new wxNotebook(_viewPanel, wxID_ANY);

	// And we add both pieces to the main sizer
	_viewSizer->Add(_viewSideSizer, 1, wxGROW | wxLEFT | wxTOP | wxBOTTOM, kMacMargins);
	_viewSizer->Add(_viewBook, 2, wxGROW | wxALL, kMacMargins);

	// Which we set as the panel sizer for the main view panel
	_viewPanel->SetSizer(_viewSizer);
}

void PreviewFrame::onViewDirFileChanged(wxCommandEvent &event) {
	// First we need to know if what was selected was a folder or a file
	wxFileName filename = wxFileName(_viewDir->GetFilePath());
	if (filename.IsDir() == false) {
		wxString fileName = _viewDir->GetFilePath().AfterLast('/');
		wxString fileExt = fileName.AfterLast('.').Lower();
		Ext ext = EXT_null;

		// If the filename has no extension, we consider it to be an executable
		if (fileName.IsSameAs(fileExt)) {
			ext = EXT_exe;
		
		// Otherwise, as long as it has a recognized extension, we can grab the EXT int
		} else if (_fileExt.find(fileExt) != _fileExt.end()) {
			ext = _fileExt.at(fileExt);
		}

		// If it's a file, we want to get the name
		if (fileExt.IsSameAs(fileName.Lower())) {
			_viewFileName->SetLabel(fileName);

		} else {
			_viewFileName->SetLabel(fileName.BeforeLast('.'));
		}
		
		// The type
		if (fileName.IsSameAs("Makefile")) {
			_viewFileType->SetLabel("Makefile");
		
		} else if (_fileExtString.find(ext) != _fileExtString.end()) {
			_viewFileType->SetLabel(_fileExtString.at(ext));
		}

		// And the size
		wxFile *file = new wxFile(_viewDir->GetFilePath(), wxFile::read);
		_viewFileSize->SetLabel(wxString::Format("%lldb", file->Length()));
		file->Close();

		// And finally we can layout the sizer to make sure the text gets aligned
		_viewDirBox->Layout();
	}
}

// This is the big important function, for when a file in the directory is double clicked
void PreviewFrame::onViewDirFileChosen(wxCommandEvent &event) {
	wxBoxSizer *newFileSizer = new wxBoxSizer(wxVERTICAL);
	wxString fileName = _viewDir->GetFilePath().AfterLast('/');
	wxString fileExt = fileName.AfterLast('.').Lower();

	// Now that we know the name, we can check if the file is already open
	for (int i = 0; i < _viewBook->GetPageCount(); i++) {
		if (_viewBook->GetPageText(i).IsSameAs(fileName)) {

			// If the file is already open, we can just set the selection to be that file
			// *note* this means identically named files in separate directories won't open...
			// Solve this by having a vector of all the paths to the files
			_viewBook->SetSelection(i);
			return;
		}
	}

	// If it is not open, we want a panel to put the contents on,
	 wxPanel *newFile = new wxPanel(_viewBook, wxID_ANY);

	// And a close button
	wxButton *closeFile = new wxButton(newFile, wxID_ANY, "Close", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxBORDER_NONE, wxDefaultValidator, wxEmptyString);
			  closeFile->SetBitmap(wxArtProvider::GetIcon(wxART_CLOSE, wxART_BUTTON));
			  closeFile->Bind(wxEVT_BUTTON, &PreviewFrame::onCloseFile, this);

	// We also need a sizer for the toolbar of options
	// And we add the apply and close buttons to the sizer right away
	wxBoxSizer *newFileToolbar = new wxBoxSizer(wxHORIZONTAL);
	newFileToolbar->Add(closeFile, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	// Then we add the toolbar to the file sizer before anything else
	newFileSizer->Add(newFileToolbar, 0, wxALIGN_RIGHT | wxRIGHT | wxLEFT | wxBOTTOM, 5);

	// Make files are special because they have no extension but are *not* executable files (although they kind of are I guess?)
	if (fileName.IsSameAs("Makefile")) {
		openTextFile(newFile, newFileToolbar, newFileSizer);

	// The other exception are executable files on unix, which have no file extension (and also any file not in the file extension map)
	} else if (_fileExt.find(fileExt) == _fileExt.end()) {
		openExecutableFile(newFile, newFileToolbar, newFileSizer);
	
	// Everything else should be available in the file extension map
	} else {
		switch (_fileExt.at(fileExt)) {
			case EXT_code:
			case EXT_asm:
			case EXT_c:
			case EXT_cpp:
			case EXT_cs:
			case EXT_cob:
			case EXT_for:
			case EXT_go:
			case EXT_head:
			case EXT_java:
			case EXT_lisp:
			case EXT_pas:
			case EXT_php:
			case EXT_html:
			case EXT_py:
			case EXT_r:
			case EXT_ruby:
			case EXT_vb:
			case EXT_text:
				openTextFile(newFile, newFileToolbar, newFileSizer);
				break;
			case EXT_exe:
			case EXT_object:
			case EXT_snes:
				openExecutableFile(newFile, newFileToolbar, newFileSizer);
				break;
			case EXT_bin:
				openBinaryFile(newFile, newFileToolbar, newFileSizer);
				break;
			case EXT_pal:
				openPaletteFile(newFile, newFileToolbar, newFileSizer);
				break;
			case EXT_gfx:
				openGfxFile(newFile, newFileToolbar, newFileSizer);
				break;
			case EXT_img:
				openImageFile(newFile, newFileToolbar, newFileSizer);
				break;
			case EXT_patch:
				openPatchFile(newFile, newFileToolbar, newFileSizer);
				break;
			case EXT_bit:
				openBitsFile(newFile, newFileToolbar, newFileSizer);
				break;
			default:
				debug("somehow we have an extension in the dictionary that is unaccounted for");
		}

	}

	// Now we can set the sizer for the panel, add the panel as a page to the notebook, and set the new selection
	newFile->SetSizer(newFileSizer);
	_viewBook->AddPage(newFile, fileName);
	_viewBook->SetSelection(_viewBook->GetPageCount() - 1);
}

void PreviewFrame::onCloseFile(wxCommandEvent &event) {
	// We need to get the button that caused the event
	wxButton *closeButton = (wxButton *) event.GetEventObject();

	// And the parent of that button is the page we want to remove
	wxWindow *file = closeButton->GetParent();

	// From which we get the page number
	int pageNum = _viewBook->FindPage(file);

	// Before deleting the page, we want to 'hide' it with an effect
	//file->HideWithEffect(wxSHOW_EFFECT_EXPAND);

	// And can finally delete the page
	_viewBook->DeletePage(pageNum);
}

// ---------
// Functions for opening each file type
// ---------
void PreviewFrame::openExecutableFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer) {
	// For an executable file, we want to create a hex editor
	wxButton *saveFile = new wxButton(panel, wxID_ANY, "Apply", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxBORDER_NONE, wxDefaultValidator, wxEmptyString);
			  saveFile->SetBitmap(wxArtProvider::GetIcon(wxART_TICK_MARK, wxART_BUTTON));

	wxStaticText *goToText = new wxStaticText(panel, wxID_ANY, "GoTo Address");
	  wxTextCtrl *goToCtrl = new wxTextCtrl(panel, wxID_ANY, "0000000", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxEmptyString);

	wxCheckBox *gridLines = new wxCheckBox(panel, wxID_ANY, "Gridlines", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, wxDefaultValidator, wxEmptyString);
				gridLines->SetValue(true);

	wxString viewAsChoices[2] = {"Bytes", "Strings"};
	wxStaticText *viewAsText = new wxStaticText(panel, wxID_ANY, "View As");
	wxChoice *viewAs = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, viewAsChoices, 0, wxDefaultValidator, wxEmptyString);

	toolbar->Insert(0, saveFile, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	toolbar->Insert(0, gridLines, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	toolbar->Insert(0, viewAs, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	toolbar->Insert(0, viewAsText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	toolbar->Insert(0, goToCtrl, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	toolbar->Insert(0, goToText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	HexViewTable *hexTable = new HexViewTable(_viewDir->GetFilePath());

	wxGrid *header = new wxGrid(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
			header->SetRowLabelSize(0);
			header->DisableDragColMove();
			header->DisableDragRowMove();
			header->DisableDragColSize();
			header->DisableDragGridSize();
			header->DisableDragRowSize();
			header->DisableCellEditControl();
			header->EnableEditing(false);
			header->UseNativeColHeader(true);
			header->ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);
			header->CreateGrid(0, 18, wxGrid::wxGridSelectionModes::wxGridSelectNone);

	// Now we can set the label size and value for each column of the header
	header->SetColLabelValue(0, "Offset");
	for (int i = 1; i < 17; i++) {
		header->SetColLabelValue(i, wxString::Format("%02X", i - 1));
		header->SetColSize(i, header->GetTextExtent("0000").GetWidth());
	}

	// There is one more column so that it matches the position of the scroll bar
	header->SetColLabelValue(17, "");

	// Magic number /2 +1
	header->SetColSize(17, (header->GetTextExtent("0000").GetWidth() / 2) + 1);

	HexGrid *grid = new HexGrid(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_DOUBLE, wxEmptyString, saveFile, goToCtrl, gridLines, viewAs);
			 grid->SetTable(hexTable, false);
			 grid->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
			 grid->SetRowLabelSize(0);
			 grid->SetColLabelSize(0);
			 grid->DisableDragColMove();
			 grid->DisableDragRowMove();
			 grid->DisableDragColSize();
			 grid->DisableDragGridSize();
			 grid->DisableDragRowSize();

	// Scrolling should be based on the size of a row
	grid->SetScrollLineY(grid->GetDefaultRowSize());

	// The size of the offset box needs to be big enough to hold the largest offset, ie. the size of the file
	wxSize offsetNumSize = header->GetTextExtent(wxString::Format("%lld", hexTable->_hexFile->_size / 16) << "00");
	int offsetSize = (offsetNumSize.GetWidth() >= header->GetTextExtent("Offset").GetWidth()) ? offsetNumSize.GetWidth() : header->GetTextExtent("Offset").GetWidth();
	grid->SetColSize(0, offsetSize);
	header->SetColSize(0, offsetSize);

	// Now we set the column width to be the size of 2 digits
	for (int i = 1; i < 17; i++) {
		grid->SetColSize(i, header->GetTextExtent("0000").GetWidth());
	}

	// The font for the offset needs to be uniform, so we need to set it to a teletype font
	wxFont f = grid->GetDefaultCellFont();
	f.SetFamily(wxFONTFAMILY_TELETYPE);
	f.SetWeight(wxFONTWEIGHT_EXTRABOLD);
	wxGridCellAttr* cellAttr = new wxGridCellAttr();
	cellAttr->SetReadOnly(true);
	cellAttr->SetFont(f);
	grid->SetColAttr(0, cellAttr);

	// HATE HATE HATE HATE HATE
	// The grid does not set a correct minimum size when you change the column widths, so we have to
	// manually calculate the minimum width of the grid based on the number of columns and their sizes, + the scrollbar
	grid->SetMinSize(wxSize((offsetSize + (header->GetTextExtent("0000").GetWidth() * 16) + (header->GetTextExtent("0000").GetWidth() / 2) + 3), grid->GetMinHeight()));

	sizer->Add(header, 0, wxALIGN_CENTER_HORIZONTAL | wxLEFT, 1);
	sizer->Add(grid, 0, wxALIGN_CENTER_HORIZONTAL);

}

void PreviewFrame::openTextFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer) {
	// We need a save button for text files
	wxButton *saveFile = new wxButton(panel, wxID_ANY, "Apply", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxBORDER_NONE, wxDefaultValidator, wxEmptyString);
			  saveFile->SetBitmap(wxArtProvider::GetIcon(wxART_TICK_MARK, wxART_BUTTON));

	// We also want a search control
	wxSearchCtrl *searchFile = new wxSearchCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, wxEmptyString);

	// Next we want word wrap
	wxCheckBox *wordWrap = new wxCheckBox(panel, wxID_ANY, "Word Wrap", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxEmptyString);

	// And an option for dark mode
	wxString darkModeChoices[2] = {"Light Mode", "Dark Mode"};
	wxChoice *darkMode = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, darkModeChoices, 0, wxDefaultValidator, wxEmptyString);

	toolbar->Insert(0, saveFile, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	toolbar->Insert(0, searchFile, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	toolbar->Insert(0, wordWrap, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	toolbar->Insert(0, darkMode, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	// Now finally we make the Scintilla backed text control by loading our file contents into it
	TextView *fileContents = new TextView(panel, ID_STC , wxDefaultPosition, wxDefaultSize, 0, wxEmptyString, _viewDir->GetFilePath(), saveFile, searchFile, wordWrap, darkMode);
			  fileContents->setupTextView();
			  fileContents->LoadFile(_viewDir->GetFilePath());

	// And then adding it to the panel sizer
	sizer->Add(fileContents, 1, wxGROW);
}

void PreviewFrame::openPaletteFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer) {
	// We need a save button for palette files
	wxButton *saveFile = new wxButton(panel, wxID_ANY, "Apply", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxBORDER_NONE, wxDefaultValidator, wxEmptyString);
			  saveFile->SetBitmap(wxArtProvider::GetIcon(wxART_TICK_MARK, wxART_BUTTON));

	// And a spin control for the bitdepth
	wxStaticText *bitdepthText = new wxStaticText(panel, wxID_ANY, "BitDepth ");
	wxSpinCtrl *bitdepth = new wxSpinCtrl(panel, wxID_ANY, "5", wxDefaultPosition, wxDefaultSize, 0, 1, 8, 0, wxEmptyString);

	// We also want a selected colour box, containing RGB values of the current selection
	wxStaticBoxSizer *selectedClr = new wxStaticBoxSizer(wxHORIZONTAL, panel, wxEmptyString);
	wxStaticText *rText = new wxStaticText(selectedClr->GetStaticBox(), wxID_ANY, "R");
	wxSpinCtrl *rSpin = new wxSpinCtrl(selectedClr->GetStaticBox(), wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, 0, 31, 0, wxEmptyString);
	rSpin->SetBase(16);

	wxStaticText *gText = new wxStaticText(selectedClr->GetStaticBox(), wxID_ANY, "G");
	wxSpinCtrl *gSpin = new wxSpinCtrl(selectedClr->GetStaticBox(), wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, 0, 31, 0, wxEmptyString);
	gSpin->SetBase(16);

	wxStaticText *bText = new wxStaticText(selectedClr->GetStaticBox(), wxID_ANY, "B");
	wxSpinCtrl *bSpin = new wxSpinCtrl(selectedClr->GetStaticBox(), wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, 0, 31, 0, wxEmptyString);
	bSpin->SetBase(16);

	wxStaticText *nText = new wxStaticText(selectedClr->GetStaticBox(), wxID_ANY, wxEmptyString);
	wxSpinCtrl *nSpin = new wxSpinCtrl(selectedClr->GetStaticBox(), wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, 0, 63, 0, wxEmptyString);
	nSpin->SetBase(16);

	selectedClr->Add(rText, 0, wxALIGN_CENTER_VERTICAL);
	selectedClr->Add(rSpin, 0, wxGROW);
	selectedClr->Add(gText, 0, wxALIGN_CENTER_VERTICAL);
	selectedClr->Add(gSpin, 0, wxGROW);
	selectedClr->Add(bText, 0, wxALIGN_CENTER_VERTICAL);
	selectedClr->Add(bSpin, 0, wxGROW);
	selectedClr->Add(nText, 0, wxALIGN_CENTER_VERTICAL);
	selectedClr->Add(nSpin, 0, wxGROW);

	// Now we can add all of them to the sizer
	toolbar->Insert(0, saveFile, 0, wxGROW | wxLEFT, 5);
	toolbar->Insert(0, bitdepth, 0, wxALIGN_CENTER_VERTICAL);
	toolbar->Insert(0, bitdepthText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	toolbar->Insert(0, selectedClr, 0, wxGROW);

	PaletteViewTable *table = new PaletteViewTable(_viewDir->GetFilePath(), saveFile, bitdepth, rSpin, gSpin, bSpin, nSpin);

	wxGrid *grid = new wxGrid(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_DOUBLE, wxEmptyString);
	grid->SetTable(table, false);
	grid->Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &PreviewFrame::onPaletteViewDClick, this);
	grid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &PreviewFrame::onPaletteViewClick, this);
	grid->Bind(wxEVT_GRID_RANGE_SELECT, &PreviewFrame::onPaletteViewClickMultiple, this);
	grid->Bind(wxEVT_KEY_DOWN, &PreviewFrame::onPaletteViewCopyPaste, this);

	// We need to set up some properties of the grid
	grid->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
	grid->SetRowLabelSize(0);
	grid->SetColLabelSize(0);
	grid->DisableDragColMove();
	grid->DisableDragRowMove();
	grid->DisableDragColSize();
	grid->DisableDragGridSize();
	grid->DisableDragRowSize();
	grid->EnableGridLines(false);
	grid->DisableCellEditControl();
	grid->EnableEditing(false);
	grid->SetDefaultRowSize(grid->GetTextExtent("0000").GetWidth());
	grid->SetDefaultColSize(grid->GetTextExtent("0000").GetWidth());
	grid->EnableGridLines(false);

	// Scrolling should be based on the size of a row
	grid->SetScrollLineY(grid->GetDefaultRowSize());

	for (int i = 0; i < 16; i++) {
		wxGridCellAttr *cellAttr = grid->GetOrCreateCellAttr(0, i);
		wxGridCellRenderer *renderer = new PaletteViewRenderer();
		cellAttr->SetRenderer(renderer);
		grid->SetColAttr(i, cellAttr);
	}
	grid->ForceRefresh();

	sizer->Add(grid, 0, wxALIGN_CENTER_HORIZONTAL);
}

void PreviewFrame::onPaletteViewCopyPaste(wxKeyEvent &event) {
	wxWindow *gridWindow = (wxWindow *) event.GetEventObject();
	wxGrid *grid = (wxGrid *) gridWindow->GetParent();
	PaletteViewTable *gridTable = (PaletteViewTable *) grid->GetTable();

	if ((event.GetModifiers() == wxMOD_CONTROL) && (event.GetKeyCode() == 'C')) {
		gridTable->_paletteFile->_copyMatrix.clear();
		if (gridTable->_paletteFile->_lastTop != -1) {
			if ((gridTable->_paletteFile->_lastBottom != -1) || (gridTable->_paletteFile->_lastRight != -1)) {
				for (int i = gridTable->_paletteFile->_lastTop; i < gridTable->_paletteFile->_lastBottom + 1; i++) {
					wxVector<wxColour> newRow;
					for (int j = gridTable->_paletteFile->_lastLeft; j < gridTable->_paletteFile->_lastRight + 1; j++) {
						newRow.push_back(gridTable->GetColour(i, j));
					}
					gridTable->_paletteFile->_copyMatrix.push_back(newRow);
				}

			} else {
				wxVector<wxColour> newColour;
				newColour.push_back(gridTable->GetColour(gridTable->_paletteFile->_lastTop, gridTable->_paletteFile->_lastLeft));
				gridTable->_paletteFile->_copyMatrix.push_back(newColour);
			}
		}

	} else if ((event.GetModifiers() == wxMOD_CONTROL) && (event.GetKeyCode() == 'V')) {
		if (gridTable->_paletteFile->_copyMatrix.size() > 0) {
			for (int y = 0; y < gridTable->_paletteFile->_copyMatrix.size(); y++) {
				for (int x = 0; x < gridTable->_paletteFile->_copyMatrix[y].size(); x++) {
					gridTable->SetColour(gridTable->_paletteFile->_lastTop + y, gridTable->_paletteFile->_lastLeft + x, gridTable->_paletteFile->_copyMatrix[y][x]);
				}
			}
			grid->ForceRefresh();
		}

	}
	
	event.Skip();
}

void PreviewFrame::onPaletteViewDClick(wxGridEvent &event) {
	int row = event.GetRow();
	int col = event.GetCol();
	wxGrid *grid = (wxGrid *) event.GetEventObject();
	PaletteViewTable *gridTable = (PaletteViewTable *) grid->GetTable();
	int bitDepth = gridTable->_paletteFile->_bitdepth;

	// First we need the colour from the grid
	wxColour colour = gridTable->GetColour(row, col);

	int red = colour.Red();
	int green = colour.Green();
	int blue = colour.Blue();

	// This colour data is then converted from whatever bitdepth it currently is, into 24bit for the colour picker
	red   = trunc(float(red)   / float((1 << bitDepth) - 1) * float((1 << 8) - 1) + 0.5f);
	green = trunc(float(green) / float((1 << bitDepth) - 1) * float((1 << 8) - 1) + 0.5f);
	blue  = trunc(float(blue)  / float((1 << bitDepth) - 1) * float((1 << 8) - 1) + 0.5f);

	// To start the dialog with a certain colour, we need to put it into a colourData object
	wxColourData *cellClrData = new wxColourData();
	cellClrData->SetColour(wxColour(red, green, blue));

	// Show the colour dialog with the cell colour
	wxColourDialog *clrDialog = new wxColourDialog(_viewPanel, cellClrData);
	clrDialog->ShowModal();

	// Get the current colour data from the dialog
	wxColourData clrData = clrDialog->GetColourData();
	wxColour newClr = clrData.GetColour();

	// Now that we have our colour, we can delete the dialog
	clrDialog->Destroy();

	// If the current colours have changed at all from what the colour picker was given, we want to write the new ones into the cell
	if ((red != newClr.Red()) || (green != newClr.Green()) || (blue != newClr.Blue())) {
		// Convert it back down to whatever bitdepth it started as
		int newR = trunc(float(newClr.Red())   / float((1 << 8) - 1) * float((1 << bitDepth) - 1) + 0.5f);
		int newG = trunc(float(newClr.Green()) / float((1 << 8) - 1) * float((1 << bitDepth) - 1) + 0.5f);
		int newB = trunc(float(newClr.Blue())  / float((1 << 8) - 1) * float((1 << bitDepth) - 1) + 0.5f);

		// And finally create a colour with those values and set it to the grid
		gridTable->SetColour(row, col, wxColour(newR, newG, newB));
	}

	grid->ForceRefresh();
}

void PreviewFrame::onPaletteViewClickMultiple(wxGridRangeSelectEvent &event) {
	wxGrid *grid = (wxGrid *) event.GetEventObject();
	PaletteViewTable *gridTable = (PaletteViewTable *) grid->GetTable();

	if (event.Selecting() == true) {
		gridTable->_paletteFile->_lastTop = event.GetTopRow();
		gridTable->_paletteFile->_lastLeft = event.GetLeftCol();
		gridTable->_paletteFile->_lastRight = event.GetRightCol();
		gridTable->_paletteFile->_lastBottom = event.GetBottomRow();
	}

	event.Skip();
}

void PreviewFrame::onPaletteViewClick(wxGridEvent &event) {
	int row = event.GetRow();
	int col = event.GetCol();
	wxGrid *grid = (wxGrid *) event.GetEventObject();
	PaletteViewTable *gridTable = (PaletteViewTable *) grid->GetTable();
	int bitDepth = gridTable->_paletteFile->_bitdepth;

	gridTable->_rSpin->SetRange(1, std::pow(2, bitDepth) - 1);
	gridTable->_gSpin->SetRange(1, std::pow(2, bitDepth) - 1);
	gridTable->_bSpin->SetRange(1, std::pow(2, bitDepth) - 1);

	wxColour colour = gridTable->GetColour(row, col);

	if ((gridTable->_paletteFile->_palType == 0) && (gridTable->_paletteFile->_palTPLFormat == 1)) {
		// Colour to get is the row * row size + column
		long colourNum = (row * 16) + col;

		// If there are fewer colours than the grid square chozen, set the value to zero
		if (colourNum > gridTable->_paletteFile->getNumColours()) {
			gridTable->_nSpin->SetValue(0);

		// Otherwise, we want to get just the index of the colour out of the palette data
		} else {
			gridTable->_nSpin->SetValue(gridTable->_paletteFile->_dataBuffer[4 + colourNum]);
		}

	} else {
		gridTable->_rSpin->SetValue(colour.Red());
		gridTable->_gSpin->SetValue(colour.Green());
		gridTable->_bSpin->SetValue(colour.Blue());	
	}

	gridTable->_paletteFile->_lastTop = row;
	gridTable->_paletteFile->_lastLeft = col;
	gridTable->_paletteFile->_lastRight = -1;
	gridTable->_paletteFile->_lastBottom = -1;

	event.Skip();
}

void PreviewFrame::openBinaryFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer) {}
void PreviewFrame::openImageFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer) {
	ImagePanel *imagePanel = new ImagePanel(panel, _viewDir->GetFilePath());

	// Keep it nice and centered at all times
	wxBoxSizer *h = new wxBoxSizer(wxHORIZONTAL);
	h->AddStretchSpacer(1);
	h->Add(imagePanel, 10, wxGROW);
	h->AddStretchSpacer(1);

	sizer->Add(h, 2, wxGROW);
}

void PreviewFrame::openPatchFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer) {}
void PreviewFrame::openBitsFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer) {}
void PreviewFrame::openGfxFile(wxPanel *panel, wxBoxSizer *toolbar, wxBoxSizer *sizer) {
	// We need a save button for palette files
	wxButton *saveFile = new wxButton(panel, wxID_ANY, "Apply", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxBORDER_NONE, wxDefaultValidator, wxEmptyString);
			  saveFile->SetBitmap(wxArtProvider::GetIcon(wxART_TICK_MARK, wxART_BUTTON));

	// And a spin control for the bits per pixel
	wxStaticText *bppText = new wxStaticText(panel, wxID_ANY, "Bits Per Pixel ");
	wxSpinCtrl *bpp = new wxSpinCtrl(panel, wxID_ANY, "5", wxDefaultPosition, wxDefaultSize, 0, 1, 8, 0, wxEmptyString);

	// Another one for the palette bits per colour
	wxStaticText *bpcText = new wxStaticText(panel, wxID_ANY, "Bits Per Colour ");
	wxSpinCtrl *bpc = new wxSpinCtrl(panel, wxID_ANY, "5", wxDefaultPosition, wxDefaultSize, 0, 1, 8, 0, wxEmptyString);

	// And of course a dropdown for the gfx type
	wxStaticText *gfxTypeText = new wxStaticText(panel, wxID_ANY, "Type ");
	wxString gfxTypeStrings[5] = {"Planar", "Planar (Composite)", "Linear", "Linear (Reversed)", "RGB"};
	wxChoice *gfxType = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, gfxTypeStrings, 0, wxDefaultValidator, wxEmptyString);

	// Now we can add all of them to the sizer
	toolbar->Insert(0, saveFile, 0, wxGROW | wxLEFT, 5);
	toolbar->Insert(0, gfxType, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	toolbar->Insert(0, gfxTypeText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	toolbar->Insert(0, bpp, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	toolbar->Insert(0, bppText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	toolbar->Insert(0, bpc, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	toolbar->Insert(0, bpcText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	// Now the side bar, where we have the image and the palette grid
	wxBoxSizer *sideSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText *rotationText = new wxStaticText(panel, wxID_ANY, "Rotation");
	wxSlider *rotation = new wxSlider(panel, wxID_ANY, 0, 0, 360, wxDefaultPosition, wxDefaultSize, wxSL_LABELS | wxSL_AUTOTICKS, wxDefaultValidator, wxEmptyString);
	wxButton *rot45 = new wxButton(panel, wxID_ANY, "45", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, wxEmptyString);
	wxButton *rot90 = new wxButton(panel, wxID_ANY, "90", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, wxEmptyString);
	wxButton *rot180 = new wxButton(panel, wxID_ANY, "180", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, wxEmptyString);
	wxButton *mirrorH = new wxButton(panel, wxID_ANY, "Mirror H", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, wxEmptyString);
	wxButton *mirrorV = new wxButton(panel, wxID_ANY, "Mirror V", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, wxEmptyString);

	GfxViewBox *viewBox = new GfxViewBox(panel, rotation, rot45, rot90, rot180, mirrorH, mirrorV);

	wxButton *loadPalette = new wxButton(panel, wxID_ANY, "Load Palette");
	wxButton *upArrow = new wxButton(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_NOTEXT | wxBORDER_NONE | wxBU_EXACTFIT, wxDefaultValidator, wxEmptyString);
			  upArrow->SetBitmap(wxArtProvider::GetIcon(wxART_GO_UP, wxART_BUTTON));

	wxButton *downArrow = new wxButton(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_NOTEXT | wxBORDER_NONE | wxBU_EXACTFIT, wxDefaultValidator, wxEmptyString);
			  downArrow->SetBitmap(wxArtProvider::GetIcon(wxART_GO_DOWN, wxART_BUTTON));

	GfxViewTable *table = new GfxViewTable(_viewDir->GetFilePath(), saveFile, bpp, bpc, gfxType, loadPalette, viewBox, upArrow, downArrow);

	wxGrid *grid = new wxGrid(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_DOUBLE, wxEmptyString);
	grid->SetTable(table, false);
	grid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &PreviewFrame::onGfxViewClick, this);
	grid->Bind(wxEVT_GRID_RANGE_SELECT, &PreviewFrame::onGfxViewClickMultiple, this);
	grid->Bind(wxEVT_KEY_DOWN, &PreviewFrame::onGfxViewCopyPaste, this);

	// We need to set up some properties of the grid
	grid->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
	grid->SetRowLabelSize(0);
	grid->SetColLabelSize(0);
	grid->DisableDragColMove();
	grid->DisableDragRowMove();
	grid->DisableDragColSize();
	grid->DisableDragGridSize();
	grid->DisableDragRowSize();
	grid->EnableGridLines(false);
	grid->DisableCellEditControl();
	grid->EnableEditing(false);
	grid->SetDefaultRowSize(grid->GetTextExtent("000").GetWidth());
	grid->SetDefaultColSize(grid->GetTextExtent("000").GetWidth());
	grid->EnableGridLines(false);

	// Scrolling should be based on the size of a row
	grid->SetScrollLineY(grid->GetDefaultRowSize());

	for (int i = 0; i < 16; i++) {
		wxGridCellAttr *cellAttr = grid->GetOrCreateCellAttr(0, i);
		wxGridCellRenderer *renderer = new GfxViewRenderer();
		cellAttr->SetRenderer(renderer);
		grid->SetColAttr(i, cellAttr);
	}
	grid->ForceRefresh();

	GfxPaletteTable *paletteTable = new GfxPaletteTable(table->_gfxFile);

	wxGrid *paletteGrid = new wxGrid(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_DOUBLE, wxEmptyString);
	paletteGrid->SetTable(paletteTable, false);
	paletteGrid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &PreviewFrame::onGfxPaletteTableClick, this);
	paletteGrid->Bind(wxEVT_GRID_CELL_LEFT_DCLICK, &PreviewFrame::onGfxPaletteTableDClick, this);

	// We need to set up some properties of the grid
	paletteGrid->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);
	paletteGrid->SetRowLabelSize(0);
	paletteGrid->SetColLabelSize(0);
	paletteGrid->DisableDragColMove();
	paletteGrid->DisableDragRowMove();
	paletteGrid->DisableDragColSize();
	paletteGrid->DisableDragGridSize();
	paletteGrid->DisableDragRowSize();
	paletteGrid->EnableGridLines(false);
	paletteGrid->DisableCellEditControl();
	paletteGrid->EnableEditing(false);
	paletteGrid->SetDefaultRowSize(grid->GetTextExtent("000").GetWidth());
	paletteGrid->SetDefaultColSize(grid->GetTextExtent("000").GetWidth());
	paletteGrid->EnableGridLines(false);

	// Scrolling should be based on the size of a row
	paletteGrid->SetScrollLineY(paletteGrid->GetDefaultRowSize());

	for (int i = 0; i < 16; i++) {
		wxGridCellAttr *cellAttr = paletteGrid->GetOrCreateCellAttr(0, i);
		wxGridCellRenderer *renderer = new GfxPaletteRenderer();
		cellAttr->SetRenderer(renderer);
		paletteGrid->SetColAttr(i, cellAttr);
	}
	paletteGrid->ForceRefresh();
	viewBox->_paletteTable = paletteTable;

	wxBoxSizer *midSizer = new wxBoxSizer(wxHORIZONTAL);
	midSizer->Add(grid);

	wxBoxSizer *paletteGridSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *paletteButtonsSizer = new wxBoxSizer(wxVERTICAL);

	paletteButtonsSizer->Add(upArrow);
	paletteButtonsSizer->Add(downArrow);

	paletteGridSizer->Add(paletteGrid);
	paletteGridSizer->Add(paletteButtonsSizer, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);

	wxBoxSizer *rotButtonSizer = new wxBoxSizer(wxHORIZONTAL);

	rotButtonSizer->Add(rot45);
	rotButtonSizer->Add(rot90, 0, wxLEFT, 5);
	rotButtonSizer->Add(rot180, 0, wxLEFT, 5);
	rotButtonSizer->Add(mirrorH, 0, wxLEFT, 5);
	rotButtonSizer->Add(mirrorV, 0, wxLEFT, 5);

	sideSizer->Add(viewBox, 0, wxGROW | wxLEFT, 10);
	sideSizer->Add(paletteGridSizer, 0, wxGROW | wxLEFT | wxTOP, 10);
	sideSizer->Add(loadPalette, 0, wxLEFT | wxTOP, 10);
	sideSizer->Add(rotationText, 0, wxGROW | wxLEFT | wxTOP, 10);
	sideSizer->Add(rotation, 0, wxGROW | wxLEFT, 10);
	sideSizer->Add(rotButtonSizer, 0, wxGROW | wxLEFT | wxTOP, 10);

	midSizer->Add(sideSizer, 0, wxGROW);
	sizer->Add(midSizer, 0, wxALIGN_CENTER_HORIZONTAL);
}

void PreviewFrame::onGfxViewClickMultiple(wxGridRangeSelectEvent &event) {
	wxGrid *grid = (wxGrid *) event.GetEventObject();
	GfxViewTable *gridTable = (GfxViewTable *) grid->GetTable();

	int left = event.GetLeftCol();
	int right = event.GetRightCol();
	int topRow = event.GetTopRow();
	int bottomRow = event.GetBottomRow();

	if (event.Selecting() == true) {
		int width = ((right - left) + 1);
		if (width > 8) {
			width = 8;
		}

		int height = ((bottomRow - topRow) + 1);
		if (height > 8) {
			height = 8;
		}

		wxImage *newImage = new wxImage(width * 8, height * 8);
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				newImage->Paste(*gridTable->GetTile(topRow + y, left + x), x * 8, y * 8);
			}
		}

		gridTable->_viewBox->_image = newImage;
		gridTable->_viewBox->Refresh();

		gridTable->_gfxFile->_lastTop = topRow;
		gridTable->_gfxFile->_lastLeft = left;
		gridTable->_gfxFile->_lastRight = right;
		gridTable->_gfxFile->_lastBottom = bottomRow;
	}

	event.Skip();
}

void PreviewFrame::onGfxViewClick(wxGridEvent &event) {
	int row = event.GetRow();
	int col = event.GetCol();
	wxGrid *grid = (wxGrid *) event.GetEventObject();
	GfxViewTable *gridTable = (GfxViewTable *) grid->GetTable();
	
	gridTable->_viewBox->_image = gridTable->GetTile(row, col);
	gridTable->_viewBox->Refresh();

	gridTable->_gfxFile->_lastTop = row;
	gridTable->_gfxFile->_lastLeft = col;
	gridTable->_gfxFile->_lastRight = -1;
	gridTable->_gfxFile->_lastBottom = -1;

	gridTable->_viewBox->_angle = 0;
	gridTable->_viewBox->_rotation->SetValue(0);

	event.Skip();
}

void PreviewFrame::onGfxPaletteTableClick(wxGridEvent &event) {
	int row = event.GetRow();
	int col = event.GetCol();
	wxGrid *grid = (wxGrid *) event.GetEventObject();
	GfxPaletteTable *gridTable = (GfxPaletteTable *) grid->GetTable();

	gridTable->_lastRow = row;
	gridTable->_lastCol = col;

	event.Skip();
}

void PreviewFrame::onGfxPaletteTableDClick(wxGridEvent &event) {
	int row = event.GetRow();
	int col = event.GetCol();
	wxGrid *grid = (wxGrid *) event.GetEventObject();
	GfxPaletteTable *gridTable = (GfxPaletteTable *) grid->GetTable();
	int bitDepth = gridTable->_gfxFile->_bitsPerColour;

	// First we need the colour from the grid
	wxColour colour = gridTable->GetColour(row, col);

	int red = colour.Red();
	int green = colour.Green();
	int blue = colour.Blue();

	// This colour data is then converted from whatever bitdepth it currently is, into 24bit for the colour picker
	red   = trunc(float(red)   / float((1 << bitDepth) - 1) * float((1 << 8) - 1) + 0.5f);
	green = trunc(float(green) / float((1 << bitDepth) - 1) * float((1 << 8) - 1) + 0.5f);
	blue  = trunc(float(blue)  / float((1 << bitDepth) - 1) * float((1 << 8) - 1) + 0.5f);

	// To start the dialog with a certain colour, we need to put it into a colourData object
	wxColourData *cellClrData = new wxColourData();
	cellClrData->SetColour(wxColour(red, green, blue));

	// Show the colour dialog with the cell colour
	wxColourDialog *clrDialog = new wxColourDialog(_viewPanel, cellClrData);
	clrDialog->ShowModal();

	// Get the current colour data from the dialog
	wxColourData clrData = clrDialog->GetColourData();
	wxColour newClr = clrData.GetColour();

	// Now that we have our colour, we can delete the dialog
	clrDialog->Destroy();

	// If the current colours have changed at all from what the colour picker was given, we want to write the new ones into the cell
	if ((red != newClr.Red()) || (green != newClr.Green()) || (blue != newClr.Blue())) {
		// Convert it back down to whatever bitdepth it started as
		int newR = trunc(float(newClr.Red())   / float((1 << 8) - 1) * float((1 << bitDepth) - 1) + 0.5f);
		int newG = trunc(float(newClr.Green()) / float((1 << 8) - 1) * float((1 << bitDepth) - 1) + 0.5f);
		int newB = trunc(float(newClr.Blue())  / float((1 << 8) - 1) * float((1 << bitDepth) - 1) + 0.5f);

		// And finally create a colour with those values and set it to the grid
		gridTable->SetColour(row, col, wxColour(newR, newG, newB));
	}

	grid->ForceRefresh();
}

void PreviewFrame::onGfxViewCopyPaste(wxKeyEvent &event) {
	wxWindow *gridWindow = (wxWindow *) event.GetEventObject();
	wxGrid *grid = (wxGrid *) gridWindow->GetParent();
	GfxViewTable *gridTable = (GfxViewTable *) grid->GetTable();

	if ((event.GetModifiers() == wxMOD_CONTROL) && (event.GetKeyCode() == 'C')) {
		gridTable->_gfxFile->_copyMatrix.clear();
		if (gridTable->_gfxFile->_lastTop != -1) {
			if ((gridTable->_gfxFile->_lastBottom != -1) || (gridTable->_gfxFile->_lastRight != -1)) {
				for (int i = gridTable->_gfxFile->_lastTop; i < gridTable->_gfxFile->_lastBottom + 1; i++) {
					wxVector<wxImage> newRow;
					for (int j = gridTable->_gfxFile->_lastLeft; j < gridTable->_gfxFile->_lastRight + 1; j++) {
						newRow.push_back(*gridTable->GetTile(i, j));
					}
					gridTable->_gfxFile->_copyMatrix.push_back(newRow);
				}

			} else {
				wxVector<wxImage> newColour;
				newColour.push_back(*gridTable->GetTile(gridTable->_gfxFile->_lastTop, gridTable->_gfxFile->_lastLeft));
				gridTable->_gfxFile->_copyMatrix.push_back(newColour);
			}
		}

	} else if ((event.GetModifiers() == wxMOD_CONTROL) && (event.GetKeyCode() == 'V')) {
		if (gridTable->_gfxFile->_copyMatrix.size() > 0) {
			for (int y = 0; y < gridTable->_gfxFile->_copyMatrix.size(); y++) {
				for (int x = 0; x < gridTable->_gfxFile->_copyMatrix[y].size(); x++) {
					gridTable->SetTile(gridTable->_gfxFile->_lastTop + y, gridTable->_gfxFile->_lastLeft + x, gridTable->_gfxFile->_copyMatrix[y][x]);
				}
			}
			grid->ForceRefresh();
		}

	}
	
	event.Skip();
}
// ------------------------------------------------------------------

// ------------------------------------------------------------------
/* ****              		   	  ****
 * **** General purpose Functions ****
 * ****              		   	  ****
 */

/* ****              		       ****
 * **** Notebook related Functions ****
 * ****              		       ****
 */

/* ****              		      ****
 * **** Button response Functions ****
 * ****              		      ****
 */
// ------------------------------------------------------------------
































