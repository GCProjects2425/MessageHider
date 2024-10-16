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

	//Font
	HFONT hFont1 = CreateFont((-24 * width) / Interface::refWidth, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
		VARIABLE_PITCH | FF_SWISS, TEXT("Segoe UI"));

	m_hElement = CreateWindow(m_type, m_text, m_style | DS_SETFONT,
		(m_x * width) / Interface::refWidth,    // Position X
		(m_y * height) / Interface::refHeight,  // Position Y
		(m_width * width) / Interface::refWidth,  // Largeur
		(m_height * height) / Interface::refHeight,  // Hauteur
		m_parentWnd, (HMENU)m_id, (HINSTANCE)GetWindowLongPtr(m_parentWnd, GWLP_HINSTANCE), NULL);

	
	// Apply Font
	if (m_hElement) {
		SendMessage(m_hElement, WM_SETFONT, (WPARAM)hFont1, TRUE);
	}

}

