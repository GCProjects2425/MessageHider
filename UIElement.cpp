#include "UIElement.h"
#include "Interface.h"

UIElement::UIElement(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text)
{
	m_parentWnd = hParentWnd;
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	m_id = id;
	m_text = text;

	m_type = nullptr;
	m_style = NULL;

	m_hElement = nullptr;
}

void UIElement::CreateElement()
{
	RECT rect;
	int width = m_width;
	int height = m_height;

	if (GetWindowRect(m_parentWnd, &rect))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}

	
	m_hElement = CreateWindow(m_type, m_text, m_style,
		(m_x * width)/ Interface::refWidth, //Pos X
		(m_y * height) / Interface::refHeight, //Pos Y
		(m_width * width) / Interface::refWidth, //Width
		(m_height * height) / Interface::refHeight, //Height
		m_parentWnd, (HMENU)m_id, (HINSTANCE)GetWindowLongPtr(m_parentWnd, GWLP_HINSTANCE), NULL);
}

