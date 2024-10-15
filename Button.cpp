#include "Button.h"


Button::~Button() {}


void Button::HandleCommand(WPARAM wParam) {
    if (LOWORD(wParam) == m_id) {
        MessageBox(NULL, L"Button clicked!", L"Info", MB_OK);
    }
}
