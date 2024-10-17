#pragma once

#include "UIElement.h"
#include <vector>
#include "framework.h"



class Interface
{
public:
    Interface(HWND parent) : parentWindow(parent) { ChangeTheme(parent, ID_VIEW_DARKMODE); }

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

    void ChangeTheme(HWND hWnd, UINT message);


private:
    HWND parentWindow;

    static COLORREF elementColor;  // Couleur de fond d'element
    static COLORREF borderColor;  // Couleur de la bordure
    static COLORREF bckgdColor;  // Couleur de background
    static COLORREF textColor; // Couleur du texte

};


