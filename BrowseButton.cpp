#include "BrowseButton.h"

void BrowseButton::HandleCommand(WPARAM wParam)
{
    
}

void BrowseButton::BrowseForFile()
{
    //{

    //    int wmId = LOWORD(wParam);
    //    if (wmId == 1)  // Si le bouton est cliqué
    //    {

    //    }
    //    switch (wmId)
    //    {
    //    case IDM_ABOUT:
    //        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
    //        break;
    //    case IDM_EXIT:
    //        DestroyWindow(hWnd);
    //        break;
    //    case 1:
    //    {
    //        OPENFILENAME ofn;
    //        wchar_t file_name[100] = { 0 };
    //        ZeroMemory(&ofn, sizeof(OPENFILENAME));
    //        ofn.lStructSize = sizeof(OPENFILENAME);
    //        ofn.hwndOwner = hWnd;
    //        ofn.lpstrFile = file_name;
    //        ofn.nMaxFile = 100;
    //        ofn.lpstrFilter = L"Images\0*.bmp;*.jpg;*.png;*.gif\0";
    //        ofn.nFilterIndex = 1;
    //        ofn.lpstrTitle = L"Sélectionnez une image";
    //        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    //        if (GetOpenFileName(&ofn))
    //        {
    //            if (imageHandler->Load(file_name))
    //            {
    //                InvalidateRect(hWnd, NULL, TRUE);
    //            }
    //        }
    //    }
    //    break;
    //    case 2:
    //        imageHandler->Write();
    //        break;
    //    default:
    //        return DefWindowProc(hWnd, message, wParam, lParam);
    //    }
}
