#pragma once
#include "UIElement.h"

class Field : public UIElement
{
public:
	Field(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text)
		: UIElement(hParentWnd, x, y, width, height, id, text) {
		m_type = L"STATIC";
	};
	~Field() {};


protected:
};

