#pragma once

#include "UIElement.h"
#include <vector>

class Interface
{
public:
    Interface(HWND parent) : parentWindow(parent) {}

    ~Interface();

    void CreateInterface();
    void HandlePaints(UINT message);

    std::vector<UIElement*> elements;

    static const int refWidth = 1920;
    static const int refHeight = 1080;

    static float scale;


private:
    HWND parentWindow;

};


