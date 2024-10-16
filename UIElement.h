#pragma once
#include <windows.h>

class UIElement
{
public:
	
	UIElement(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text);
	~UIElement() {};

	virtual void CreateElement();
	virtual void HandlePaint(UINT message) {};

	int m_id;
	HWND m_hElement;

protected:
	

	HWND m_parentWnd;
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	
	LPCWSTR m_text;

	const wchar_t* m_type;
	long m_style;
};

