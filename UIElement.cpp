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

	// Création de la police
	HFONT hFont1 = CreateFont((-24 * width) / Interface::refWidth, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
		VARIABLE_PITCH | FF_SWISS, TEXT("Segoe UI"));

	// Création de la fenêtre ou du contrôle
	m_hElement = CreateWindow(m_type, m_text, m_style | DS_SETFONT,
		(m_x * width) / Interface::refWidth,    // Position X
		(m_y * height) / Interface::refHeight,  // Position Y
		(m_width * width) / Interface::refWidth,  // Largeur
		(m_height * height) / Interface::refHeight,  // Hauteur
		m_parentWnd, (HMENU)m_id, (HINSTANCE)GetWindowLongPtr(m_parentWnd, GWLP_HINSTANCE), NULL);

	// Vérifier si la création du contrôle a réussi
	if (m_hElement) {
		// Appliquer la police au contrôle en envoyant le message WM_SETFONT
		SendMessage(m_hElement, WM_SETFONT, (WPARAM)hFont1, TRUE);
	}

}

