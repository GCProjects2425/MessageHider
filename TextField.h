#pragma once
#include "Field.h"

class TextField : public Field
{
public:
	TextField(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text)
		: Field(hParentWnd, x, y, width, height, id, text) {
		m_type = L"EDIT";
		m_style = WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL | ES_NOHIDESEL;
	};
	~TextField() {};
};

