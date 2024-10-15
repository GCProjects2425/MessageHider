#pragma once
#include "Button.h"

class BrowseButton : public Button
{
public:
	BrowseButton(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text)
		: Button(hParentWnd, x, y, width, height, id, text) 
	{
		m_type = L"BUTTON";
		m_style = WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY;

	};


	void HandleCommand(WPARAM wParam) override;

	void BrowseForFile(); // Load

	void BrowseForDirectory(); //Save

};

