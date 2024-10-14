#pragma once

#include "Button.h"
#include <vector>

class Interface
{
public:
    Interface(HWND parent) : parentWindow(parent) {}

    ~Interface();

    void CreateInterface();
    std::vector<Button> buttons;

private:
    HWND parentWindow;

};


