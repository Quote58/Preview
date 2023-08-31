#include "paletteView.h"

// Get a colour for a given row/col of the palette grid
wxColour PaletteViewTable::GetColour(int row, int col) {
	// Colour to get is the row * row size + column
	long colourNum = (row * 16) + col;

	// If there are fewer colours than the grid square chozen, return black
	if (colourNum > _paletteFile->getNumColours()) {
		return wxColour(0,0,0,0);
	
	// Otherwise, return the colour from the colour file for this index
	} else {
		return _paletteFile->getColour(colourNum);
	}
}

// Set a colour for a given row/col of the palette grid
void PaletteViewTable::SetColour(int row, int col, const wxColour &value) {
	long colourNum = (row * 16) + col;
	_paletteFile->saveColour(value, colourNum);
}

void PaletteViewTable::onSaveFile(wxCommandEvent &event) {
	_paletteFile->saveToFile();
}

void PaletteViewTable::onBitdepthChanged(wxSpinEvent &event) {
	_paletteFile->_bitdepth = event.GetPosition();
	wxGrid *grid = GetView();
	grid->ForceRefresh();
}

void PaletteViewTable::onRedChanged(wxSpinEvent &event) {
	wxGrid *grid = GetView();

	wxColour colour = GetColour(_paletteFile->_lastTop, _paletteFile->_lastLeft);
	SetColour(_paletteFile->_lastTop, _paletteFile->_lastLeft, wxColour(event.GetPosition(), colour.Green(), colour.Blue()));

	grid->ForceRefresh();
}

void PaletteViewTable::onGreenChanged(wxSpinEvent &event) {
	wxGrid *grid = GetView();

	wxColour colour = GetColour(_paletteFile->_lastTop, _paletteFile->_lastLeft);
	SetColour(_paletteFile->_lastTop, _paletteFile->_lastLeft, wxColour(colour.Red(), event.GetPosition(), colour.Blue()));

	grid->ForceRefresh();
}

void PaletteViewTable::onBlueChanged(wxSpinEvent &event) {
	wxGrid *grid = GetView();

	wxColour colour = GetColour(_paletteFile->_lastTop, _paletteFile->_lastLeft);
	SetColour(_paletteFile->_lastTop, _paletteFile->_lastLeft, wxColour(colour.Red(), colour.Green(), event.GetPosition()));

	grid->ForceRefresh();
}

void PaletteViewTable::onNESChanged(wxSpinEvent &event) {
	wxGrid *grid = GetView();

	// Colour to get is the row * row size + column
	long colourNum = (_paletteFile->_lastTop * 16) + _paletteFile->_lastLeft;

	// If there are fewer colours than the grid square chozen, return black
	if (colourNum > _paletteFile->getNumColours()) {
		return;
	
	// Otherwise, set the colour index to be the spin position
	} else {
		_paletteFile->_dataBuffer[4 + colourNum] = event.GetPosition();
	}

	grid->ForceRefresh();	
}

// Render the data as a colour for any given cell of the grid
void PaletteViewRenderer::Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected) {
	dc.SetBackgroundMode(wxBRUSHSTYLE_SOLID);

	wxColour clr;
	if (grid.IsThisEnabled()) {
		if (isSelected) {
			if (grid.HasFocus()) {
				clr = grid.GetSelectionBackground();

			} else {
				clr = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW);
			}
		
		} else {
			PaletteViewTable *table = (PaletteViewTable *) grid.GetTable();

			if (table->GetValue(row, col) == "><") {
				clr = wxColour(0,0,0,0);
				return;
			
			} else {
				int brightness = 8 - table->_paletteFile->_bitdepth;
				clr = table->GetColour(row, col);
				clr = wxColour(clr.Red() << brightness, clr.Green() << brightness, clr.Blue() << brightness);
			}
		}
	
	} else {
		// grey out fields if the grid is disabled
		clr = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
	}

	dc.SetBrush(clr);
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.DrawRectangle(rect);
}