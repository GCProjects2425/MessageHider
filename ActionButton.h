#pragma once
#include "Button.h"
class ActionButton : public Button
{
public:
	ActionButton(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text)
		: Button(hParentWnd, x, y, width, height, id, text) {
		m_type = L"BUTTON";
	};

	void HandleCommand(WPARAM wParam) override;
};

