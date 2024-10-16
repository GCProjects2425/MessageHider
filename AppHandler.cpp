#include "AppHandler.h"
#include "ErrorHandler.h"

HWND AppHandler::m_hWnd = nullptr;

void AppHandler::OpenImage()
{
    OPENFILENAME ofn;
    wchar_t file_name[100] = { 0 };
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFile = file_name;
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = L"Images\0*.bmp;*.jpg;*.png;*.gif\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrTitle = L"Sélectionner une image";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        if (ImageHandler::GetInstance()->Load(file_name))
        {
            InvalidateRect(m_hWnd, NULL, TRUE);
        }
    }
}

void AppHandler::SaveImage()
{
    // Vérification que l'image est valide avant de permettre la sauvegarde
    if (!ImageHandler::GetInstance()->isValidImage())
    {
        ErrorHandler::GetInstance()->Error(ErrorHandler::ErrorType::NO_IMAGE_LOADED);
        return;  
    }

    OPENFILENAME ofn;
    wchar_t save_file_name[100] = { 0 };
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFile = save_file_name;
    ofn.nMaxFile = sizeof(save_file_name);

    // Ajout des filtres de format d'image supportés
    ofn.lpstrFilter = L"Fichiers PNG (*.png)\0*.png\0"
        L"Fichiers JPEG (*.jpg; *.jpeg)\0*.jpg;*.jpeg\0"
        L"Fichiers BMP (*.bmp)\0*.bmp\0"
        L"Fichiers GIF (*.gif)\0*.gif\0";

    ofn.nFilterIndex = 1;  // Sélection par défaut : PNG
    ofn.lpstrDefExt = L"png";  // Extension par défaut
    ofn.lpstrTitle = L"Sauvegarder l'image";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    // Ouvre la boîte de dialogue de sauvegarde
    if (GetSaveFileName(&ofn))
    {
        ImageHandler::GetInstance()->Save(save_file_name);
    }
}

