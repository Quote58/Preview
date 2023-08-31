#include "imageView.h"

ImagePanel::ImagePanel(wxPanel *parent, wxString file) : wxPanel(parent) {
	// First thing we need to do is separate the extension from the file name
	wxString fileName = file.AfterLast('/');
	wxString fileExt = fileName.AfterLast('.');

	// Now, most image files are simple, but SVG is a different matter
	// wx doesn't have a native SVG handler, so instead we have to get a little creative
	if (fileExt.IsSameAs("svg")) {
		// If it's an svg, we want to open it up and find the width and height
		wxFile *f = new wxFile(file, wxFile::read);
		wxString imageString = "";
		
		// We start by reading the whole file into a string (which is what SVG files are anyway)
		f->ReadAll(&imageString);

		// Then we find the first instance of width/height=
		int widthIndex = imageString.First("width=");
		int heightIndex = imageString.First("height=");

		// Get the index of the end of their number (because the number is a float and can be variable length)
		int widthStringEnd = imageString.find("\"", widthIndex + 6);
		int heightStringEnd = imageString.find("\"", heightIndex + 7);

		// With this information we can get a string of the number
		wxString widthString = imageString.Mid(widthIndex + 7, widthStringEnd - (widthIndex + 1));
		wxString heightString = imageString.Mid(heightIndex + 8, heightStringEnd - (heightIndex + 1));

		// Which we convert from the string
		int width = 0;
		int height = 0;
		widthString.ToInt(&width);
		heightString.ToInt(&height);

		// And don't forget to close the file
		f->Close();

		// Now, we can't just load an SVG directly into a bmp, but we can make a bitmap bundle out of one
		wxBitmapBundle svgBundle = wxBitmapBundle::FromSVGFile(file, wxSize(width, height));

		// So we make a bundle, and then get a bitmap out of the bundle bit enough for the image panel
		wxBitmap svgBitmap = svgBundle.GetBitmapFor(parent);

		// And finally, we convert that to a wxImage which we can resize and use later
		_image = svgBitmap.ConvertToImage();

	} else {
		// If it is not an SVG, we just grab the appropriate format specifier and load the image
		if (_imgType.find(fileExt) != _imgType.end()) {
			wxBitmapType format = _imgType.at(fileExt);
			_image.LoadFile(file, format);

		// And if it is not in the list of image handlers, we can't use it
		} else {
			wxMessageBox("This image type is not recognized by Preview");
		}
	}

	// First we want to grab the width and height of the image
	_imageW = _image.GetWidth();
	_imageH = _image.GetHeight();

	// And for scaling we want the aspect ratio as well
	_aspectRatio = (double) (_imageW) / (double) _imageH;

	_width = -1;
	_height = -1;

	Bind(wxEVT_PAINT, &ImagePanel::paintEvent, this);
	Bind(wxEVT_SIZE, &ImagePanel::onSize, this);
	Bind(wxEVT_MOUSEWHEEL, &ImagePanel::onMouseWheel, this);
}

void ImagePanel::onMouseWheel(wxMouseEvent &event) {
	// If wheel rotation is positive, we want to zoom in
	if (event.GetWheelRotation() >= 0) {
		zoomIn();

	// If it is negative, we want to zoom out
	} else {
		zoomOut();
	}
	
	// Either way, we want to grab the position of the mouse
	wxPaintDC dc(this);
	wxPoint pos = event.GetLogicalPosition(dc);
	_mouseX = pos.x;
	_mouseY = pos.y;
}

void ImagePanel::paintEvent(wxPaintEvent &event) {
	wxPaintDC dc(this);
	render(dc);
}

void ImagePanel::render(wxDC &dc) {
	// The size of the dc changes when you resize the window, so the size of the image needs to adjust
	int newWidth = 0;
	int newHeight = 0;
	dc.GetSize(&newWidth, &newHeight);

	int scaledW = newWidth;
	int scaledH = (int) (scaledW / _aspectRatio);

	if (scaledH > newHeight) {
		scaledH = newHeight;
		scaledW = (int) (scaledH * _aspectRatio);
	}

	// If the window size changed, adjust the size of the image
	if ((scaledW != _width) || (scaledH != _height)) {
		_resizedImage = wxBitmap(_image.Scale(scaledW, scaledH));
		_width = scaledW;
		_height = scaledH;
	}

	// Scale the image drawing by the zoom factor
	dc.SetUserScale(_zoomFactor, _zoomFactor);

	int x = ((_width * _zoomFactor) - _width);
	int y = ((_height * _zoomFactor) - _height);

	int mX = 1;
	if ((_mouseX > 0) && (_mouseX < _height)) {
		mX = (_width * _zoomFactor) / _mouseX;
	}

	int mY = 1;
	if ((_mouseY > 0) && (_mouseY < _height)) {
		mY = (_height * _zoomFactor) / _mouseY;
	}

	// Draw the image with the current zoom
    dc.DrawBitmap(_resizedImage, 0 - (x / (mX * _zoomFactor)), 0 - (y / (mY * _zoomFactor)), false);

    // Have to reset the user scale after utilizing it so it doesn't affect future calls to draw
    dc.SetUserScale(1.0, 1.0);
}

void ImagePanel::onSize(wxSizeEvent &event) {
	Refresh();
	event.Skip();
}

void ImagePanel::zoomIn() {
    setZoom(_zoomFactor *= 1.2);
}

void ImagePanel::zoomOut() {
    setZoom(_zoomFactor /= 1.2);
}

void ImagePanel::setZoom(double zoomFactor) {
	if (zoomFactor < 1.0) {
		zoomFactor = 1.0;

	} else if (zoomFactor > 5.0) {
		zoomFactor = 5.0;
	}

	_zoomFactor = zoomFactor;
	Refresh();
}
