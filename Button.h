#pragma once

#include <windows.h>
#include "UIElement.h"

class Button : public UIElement
{
public:
    Button();
    ~Button();

    void CreateButton(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text);

    // Méthode pour gérer les événements
    void HandleCommand(WPARAM wParam);

private:
    HWND hButton;
    int buttonId;
};

