#pragma once

#include "UIElement.h"
#include <vector>


class Interface
{
public:
    Interface(HWND parent) : parentWindow(parent) { ChangeTheme(); }

    ~Interface();

    void CreateInterface();
    void HandlePaints(UINT message);
    void ApplyTheme(LPARAM lParam);

    std::vector<UIElement*> elements;

    static const int refWidth = 1920;
    static const int refHeight = 1080;

    static float scale;

    static COLORREF GetElementColor() { return elementColor; }
    static COLORREF GetBorderColor() { return borderColor; }
    static COLORREF GetBackgroundColor() { return bckgdColor; }
    static COLORREF GetTextColor() { return textColor; }

    void ChangeTheme();


private:
    HWND parentWindow;

    static COLORREF elementColor;  // Couleur de fond
    static COLORREF borderColor;  // Couleur de la bordure
    static COLORREF bckgdColor;  // Couleur de la bordure
    static COLORREF textColor; // Couleur du texte

};


