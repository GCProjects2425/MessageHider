#pragma once

#include "UIElement.h"
#include <vector>

class Interface
{
public:
    Interface(HWND parent) : parentWindow(parent) {}

    ~Interface();

    void CreateInterface();
    void HandleCommands(WPARAM wParam);

    std::vector<UIElement*> elements;

private:
    HWND parentWindow;

};


