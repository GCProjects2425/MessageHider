#include "Button.h"

Button::Button() {}

Button::~Button() {}

void Button::CreateButton(HWND hParentWnd, int x, int y, int width, int height, int id, LPCWSTR text) {
    buttonId = id;
    hButton = CreateWindow(L"BUTTON", text, WS_VISIBLE | WS_CHILD,
        x, y, width, height, hParentWnd, (HMENU)id, (HINSTANCE)GetWindowLongPtr(hParentWnd, GWLP_HINSTANCE), NULL);
}

void Button::HandleCommand(WPARAM wParam) {
    if (LOWORD(wParam) == buttonId) {
        MessageBox(NULL, L"Button clicked!", L"Info", MB_OK);
    }
}
