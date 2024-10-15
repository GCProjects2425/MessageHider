#pragma once

#include "UIElement.h"

class Button : public UIElement
{
public:
    Button(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text)
        : UIElement(hParentWnd, x, y, width, height, id, text) 
    {
        m_type = L"BUTTON";
        m_style = WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY;
    };

    ~Button();

    void HandleCommand(WPARAM wParam) override;

private:
};

