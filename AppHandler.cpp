#include "AppHandler.h"

void AppHandler::OpenImage(HWND& hWnd, ImageHandler& imageHandler)
{
    OPENFILENAME ofn;
    wchar_t file_name[100] = { 0 };
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = file_name;
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = L"Images\0*.bmp;*.jpg;*.png;*.gif\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrTitle = L"Sélectionnez une image";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        if (imageHandler.Load(file_name))
        {
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
}
