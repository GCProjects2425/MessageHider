#include "ImageField.h"
#include "AppHandler.h"

// Méthode de gestion du dessin
void ImageField::HandlePaint(UINT uMsg) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hElement, &ps);

    if (ImageHandler::GetInstance()->isValidImage()) {
        RECT rect;
        GetClientRect(m_hElement, &rect); 

        int fieldWidth = rect.right - rect.left;
        int fieldHeight = rect.bottom - rect.top;

        ImageHandler::GetInstance()->Draw(hdc, 0, 0, fieldWidth, fieldHeight);
    }

    EndPaint(m_hElement, &ps);
}
