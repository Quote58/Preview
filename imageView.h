// ImageView

#ifndef IMAGE_H
#define IMAGE_H

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include "file.h"

class ImagePanel : public wxPanel {
public:
	wxImage _image;
	wxBitmap _resizedImage;
	int _imageW = 0;
	int _imageH = 0;
	double _aspectRatio = 0;

	int _width = 0;
	int _height = 0;
	int _mouseX = 0;
	int _mouseY = 0;

	double _zoomFactor = 1.0;

	// We need a way to connect file extensions to image files
	std::unordered_map<wxString, wxBitmapType> _imgType = {{"bmp", wxBITMAP_TYPE_BMP}, {"png", wxBITMAP_TYPE_PNG}, {"jpg", wxBITMAP_TYPE_JPEG},
														   {"gif", wxBITMAP_TYPE_GIF}, {"xpm", wxBITMAP_TYPE_XPM}, {"ico", wxBITMAP_TYPE_ICO},
														   {"jpeg", wxBITMAP_TYPE_JPEG}, {"pcx", wxBITMAP_TYPE_PCX}, {"pnm", wxBITMAP_TYPE_PNM},
														   {"tiff", wxBITMAP_TYPE_TIFF}, {"tga", wxBITMAP_TYPE_TGA}, {"cur", wxBITMAP_TYPE_CUR}};

	ImagePanel(wxPanel *parent, wxString file);

	void paintEvent(wxPaintEvent &event);
	void paintNow();
	void onSize(wxSizeEvent &event);
	void render(wxDC &dc);

	void onMouseWheel(wxMouseEvent &event);
    void zoomIn();
    void zoomOut();
    void setZoom(double zoomFactor);
};

#endif