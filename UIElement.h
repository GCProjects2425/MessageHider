#pragma once
#include <windows.h>

class UIElement
{
public:
	
	UIElement(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text);
	~UIElement() {};

	virtual void CreateElement();
	virtual void HandleCommand(WPARAM wParam) {};

protected:
	HWND hElement = nullptr;

	HWND m_parentWnd;
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	int m_id;
	LPCWSTR m_text;

	const wchar_t* m_type;
};

