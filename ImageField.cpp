#include "ImageField.h"
#include "AppHandler.h"

void ImageField::HandlePaint(UINT uMsg) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hElement, &ps);

    if (ImageHandler::GetInstance()->isValidImage()) {
        RECT rect;
        GetClientRect(m_hElement, &rect); 

        int fieldWidth = rect.right - rect.left;
        int fieldHeight = rect.bottom - rect.top;

        int imgWidth = ImageHandler::GetInstance()->GetImageWidth();
        int imgHeight = ImageHandler::GetInstance()->GetImageHeight();

        float imgRatio = static_cast<float>(imgWidth) / imgHeight;
        float fieldRatio = static_cast<float>(fieldWidth) / fieldHeight;

        int newWidth, newHeight;

        if (imgRatio > fieldRatio) {
            newWidth = fieldWidth;
            newHeight = static_cast<int>(fieldWidth / imgRatio);
        } else {
            newHeight = fieldHeight;
            newWidth = static_cast<int>(fieldHeight * imgRatio);
        }

        int offsetX = (fieldWidth - newWidth) / 2;
        int offsetY = (fieldHeight - newHeight) / 2;

        ImageHandler::GetInstance()->Draw(hdc, offsetX, offsetY, newWidth, newHeight);
    }

    EndPaint(m_hElement, &ps);
}

