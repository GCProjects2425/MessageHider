#include "Interface.h"
#include "ElementsHeaders.h"


Interface::~Interface(){}
COLORREF Interface::elementColor;
COLORREF Interface::borderColor;
COLORREF Interface::bckgdColor;
COLORREF Interface::textColor;


void Interface::CreateInterface()
{
    UIElement* loadImageButton = new BrowseButton(parentWindow, 20, 20, 300, 40, IDM_OPEN_FILE, L"Load image");
    elements.push_back(loadImageButton);
    UIElement* dataField = new Field(parentWindow, 340, 20, 1020, 40, 55, L"");
    elements.push_back(dataField);
    UIElement* textField = new TextField(parentWindow, 20, 800, 1340, 200, 69, L"");
    elements.push_back(textField);
    UIElement* previewField = new ImageField(parentWindow, 20, 80, 1340, 700, IDM_IMAGE_FIELD, L"");
    elements.push_back(previewField);

    UIElement* separatorField = new Field(parentWindow, 1380, 200, 500, 2, 6, L"");
    elements.push_back(separatorField);

    UIElement* blackWhiteFilterButton = new ActionButton(parentWindow, 1400, 220, 460, 40, ID_APPLY_BLACKWHITE_FILTER, L"Apply Grayscale Filter");
    elements.push_back(blackWhiteFilterButton);
    UIElement* invertFilterButton = new ActionButton(parentWindow, 1400, 280, 460, 40, ID_APPLY_INVERT_FILTER, L"Apply Inverted Filter");
    elements.push_back(invertFilterButton);
    UIElement* blurFilterButton = new ActionButton(parentWindow, 1400, 340, 460, 40, ID_APPLY_BLUR_FILTER, L"Apply Blured Filter");
    elements.push_back(blurFilterButton);
    UIElement* satureFilterButton = new ActionButton(parentWindow, 1400, 400, 460, 40, ID_APPLY_SATURE_FILTER, L"Apply Sature Filter");
    elements.push_back(satureFilterButton);

    UIElement* resetFilterButton = new ActionButton(parentWindow, 1400, 500, 460, 40, ID_RESET_FILTER, L"Reset image");
    elements.push_back(resetFilterButton);


    UIElement* aboutField = new Field(parentWindow, 1380, 560, 500, 320, 9, L"MessageHidder build: 2024.10.1701");
    elements.push_back(aboutField);

    UIElement* encryptButton = new ActionButton(parentWindow, 1380, 900, 240, 40, IDM_HIDE_MESSAGE, L"Encrypt message");
    elements.push_back(encryptButton);
    UIElement* decryptButton = new ActionButton(parentWindow, 1640, 900, 240, 40, IDM_DECODE_MESSAGE, L"Decrypt message");
    elements.push_back(decryptButton);
    UIElement* exportButton = new BrowseButton(parentWindow, 1380, 960, 500, 40, IDM_SAVE_FILE, L"Export image");
    elements.push_back(exportButton);

    UIElement* lightThemeButton = new ActionButton(parentWindow, 1380, 20, 240, 40, ID_VIEW_LIGHTMODE, L"Light Theme");
    elements.push_back(lightThemeButton);
    UIElement* darkThemeButton = new ActionButton(parentWindow, 1640, 20, 240, 40, ID_VIEW_DARKMODE, L"Dark Theme");
    elements.push_back(darkThemeButton);
    UIElement* redThemeButton = new ActionButton(parentWindow, 1380, 80, 240, 40, ID_VIEW_REDMODE, L"Red Theme");
    elements.push_back(redThemeButton);
    UIElement* limeThemeButton = new ActionButton(parentWindow, 1640, 80, 240, 40, ID_VIEW_LIMEMODE, L"Lime Theme");
    elements.push_back(limeThemeButton);
    UIElement* purpleThemeButton = new ActionButton(parentWindow, 1380, 140, 240, 40, ID_VIEW_PURPLEMODE, L"Purple Theme");
    elements.push_back(purpleThemeButton);
    UIElement* blueThemeButton = new ActionButton(parentWindow, 1640, 140, 240, 40, ID_VIEW_BLUEMODE, L"Blue Theme");
    elements.push_back(blueThemeButton);

    for (UIElement* element : elements) 
    {
        element->CreateElement();
    }
}


void Interface::HandlePaints(UINT message)
{
    for (auto element : elements)
        element->HandlePaint(message);
}


void Interface::ApplyTheme(LPARAM lParam)
{
    LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
    
    HDC hdc = pDIS->hDC;

    HBRUSH hBrush = CreateSolidBrush(elementColor);
    HBRUSH hBgBrush = CreateSolidBrush(bckgdColor);
    HPEN hPen = CreatePen(PS_SOLID, 2, borderColor);

    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

    FillRect(hdc, &pDIS->rcItem, hBgBrush);
    RoundRect(hdc, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom, 20, 20);


    WCHAR text[256];
    GetWindowText(pDIS->hwndItem, text, sizeof(text));

    SetTextColor(hdc, textColor);
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, text, -1, &pDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);

    DeleteObject(hBrush);
    DeleteObject(hPen);
}

void Interface::ChangeTheme(HWND hWnd, UINT message)
{

    switch (message)
    {
    case ID_VIEW_LIGHTMODE:
        elementColor = RGB(232, 232, 232);
        borderColor = RGB(204, 206, 219);
        bckgdColor = RGB(245, 245, 245);
        textColor = RGB(0, 0, 0);
        break;

    case ID_VIEW_DARKMODE:
        elementColor = RGB(45, 45, 45);
        borderColor = RGB(80, 80, 80);
        bckgdColor = RGB(35, 35, 35);
        textColor = RGB(255, 255, 255);
        break;

    case ID_VIEW_REDMODE:
        elementColor = RGB(88, 0, 0);
        borderColor = RGB(66, 66, 66);
        bckgdColor = RGB(57, 0, 0);
        textColor = RGB(255, 255, 255);
        break;

    case ID_VIEW_LIMEMODE:
        elementColor = RGB(45, 45, 45);
        borderColor = RGB(175, 244, 0);
        bckgdColor = RGB(35, 35, 35);
        textColor = RGB(255, 255, 255);
        break;

    case ID_VIEW_PURPLEMODE:
        elementColor = RGB(196, 183, 215);
        borderColor = RGB(204, 206, 219);
        bckgdColor = RGB(245, 245, 245);
        textColor = RGB(51, 51, 51);
        break;

    case ID_VIEW_BLUEMODE:
        elementColor = RGB(0, 64, 82);
        borderColor = RGB(0, 33, 43);
        bckgdColor = RGB(0, 43, 54);
        textColor = RGB(240, 240, 240);
        break;

    default:
        break;
    }

    InvalidateRect(hWnd, NULL, TRUE);


}
    

