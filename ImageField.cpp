#include "ImageField.h"
#include "AppHandler.h"

void ImageField::HandleCommand(UINT uMsg) {
    switch (uMsg) {
    case WM_PAINT:
        OnPaint();
    }
}

// Méthode de gestion du dessin
void ImageField::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hElement, &ps);

    ImageHandler::GetInstance()->Draw(hdc, 0, 0);

    EndPaint(m_hElement, &ps);
}