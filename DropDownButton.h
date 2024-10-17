#pragma once
#include "Button.h"
#include <vector>

class DropDownButton : public Button
{
public:
	DropDownButton(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text, std::vector<LPARAM> choices)
		: Button(hParentWnd, x, y, width, height, id, text) {
		m_type = L"COMBOBOX";
		m_style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | BS_OWNERDRAW;
		
		for (auto choice : choices)
			SendMessage(m_hElement, CB_ADDSTRING, 0, choice);
	}
};