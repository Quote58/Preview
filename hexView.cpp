#include "hexView.h"

/* Grid Functions */
void HexGrid::onSaveFile(wxCommandEvent &event) {
	HexViewTable *table = (HexViewTable *) GetTable();

	table->_hexFile->saveToFile();	
}

void HexGrid::onGoToEnter(wxCommandEvent &event) {
	// We need offset to be 0 in case they enter an invalid string
	long offset = 0;
	sscanf(event.GetString().c_str(), "%lx", &offset);
	goToOffset(offset);
}

void HexGrid::goToOffset(long offset) {
	HexViewTable *table = (HexViewTable *) GetTable();

	if ((offset < 0) || ((offset + 16) >= table->_hexFile->_size)) {
		wxMessageBox("Invalid Offset");

	} else if ((offset + 16) < table->_hexFile->_size) {
		// We unfortunately need both X and Y for the function
		int scrollX;
		int scrollY;
		GetScrollPixelsPerUnit(&scrollX, &scrollY);

		// Before we divide the offset to get the number of rows, we want to save the number for later use
		table->_offset = offset;

		// Now we take the int of their hex string, and divide it by the number of bytes per line
		offset = offset / 16;

		// And finally we can multiply the line offset by the line size, diving by the scroll increment (which is the same as the row size anyway in this case)
		Scroll(0, (offset * GetDefaultRowSize() / scrollY));
		ForceRefresh();
	}
}

void HexGrid::onGridLinesCheck(wxCommandEvent &event) {
	EnableGridLines(event.GetInt());
	ForceRefresh();
}

void HexGrid::onViewTypeChanged(wxCommandEvent &event) {
	HexViewTable *table = (HexViewTable *) GetTable();

	if (event.GetSelection() == 0) {
		table->_viewAsString = false;
	
	} else {
		table->_viewAsString = true;
	}

	ForceRefresh();
}

void HexGrid::onScrollEnd(wxScrollWinEvent &event) {
	event.Skip();
	adjustForScroll();
}

void HexGrid::onMouseWheel(wxMouseEvent &event) {
	event.Skip();
	adjustForScroll();
}

void HexGrid::adjustForScroll() {
	int offset = 16;

	HexViewTable *table = (HexViewTable *) GetTable();
	table->_offset = table->_offset + ((GetFirstFullyVisibleRow() - (table->_offset / offset)) * offset);
}

/* Grid Table Functions */
wxString HexViewTable::GetValue(int row, int col) {
	// data to get is the row * row size + column
	long num = (row * 16) + col;

	if (col == 0) {
		// To calculate the size we need for the offset string, we need to use the biggest number we'll get
		wxString sizeString = wxString::Format("%X", (int) _hexFile->_size);
		int offset = 16;

		/* Because we can't use GetFirstVisibleRow() (we're in the table, not the grid),
		 * we have to manually determine the difference between the first visible offset
		 * and our current row offset. This is done by getting the difference between
		 * the amount before the offset (_offset / offset) in rows, and the current row.
		 * Then we can simply multiply by the size of a row, and add that to the current
		 * offset to get the offset at our current row.
		 */
		offset = _offset + ((row - (_offset / offset)) * offset);
		wxString offsetString = wxString::Format("%X", offset);

		// We also pad the string out with zeroes so it looks cleaner
		offsetString.Pad(sizeString.Len() - offsetString.Len(), '0', false);
		return offsetString;

	} else {
		long offset = _offset + ((row - (_offset / 16)) * 16) + (col - 1);

		if (_viewAsString == true) {
			return wxString::Format("%C", _hexFile->getByte(offset));
		
		} else {
			return wxString::Format("%02X", _hexFile->getByte(offset));
		}
	}
}

void HexViewTable::SetValue(int row, int col, const wxString &value) {
	// First we need the offset based on the current position
	long offset = _offset + ((row - (_offset / 16)) * 16) + (col - 1);
	int byte = -1;
	
	// Use ssccanf to get only the first two characters of hexadecimal in the input string
	sscanf(value.c_str(), "%2x", &byte);

	// If it was able to get a two digit hexadecimal value, set the byte in the rom to that
	if (byte != -1) {
		_hexFile->setByte(offset, byte);
	}

}