#pragma once
#include "UIElement.h"

class Field : public UIElement
{
public:
	Field(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text)
		: UIElement(hParentWnd, x, y, width, height, id, text) {
		m_type = L"STATIC";
		m_style = WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY;

	};
	~Field() {};
};

