﻿// MessageHider.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "MessageHider.h"
#include "ImageHandler.h"
#include "AppHandler.h"
#include "ErrorHandler.h"
#include "Interface.h"
#include "ElementsHeaders.h"
#include "framework.h"

#include <shellapi.h>
#include <dwmapi.h>

#pragma comment(lib,"Dwmapi.lib")

#define MAX_LOADSTRING 100

void EnableDarkModeForTitleBar(HWND hwnd)
{
    BOOL darkmode = TRUE;
    DwmSetWindowAttribute(hwnd, 20, &darkmode, sizeof(darkmode));
}

void ApplyTheme(HWND hwnd)
{
    HBRUSH darkBrush = CreateSolidBrush(Interface::GetBackgroundColor());
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)darkBrush);

    InvalidateRect(hwnd, NULL, TRUE);
}

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

ULONG_PTR gdiplusToken;

void InitGDIPlus()
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void ShutdownGDIPlus()
{
    GdiplusShutdown(gdiplusToken);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize GDI+.
    InitGDIPlus();

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MESSAGEHIDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MESSAGEHIDER));

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    //shutdown GDI+
    
    ShutdownGDIPlus();
    return (int) msg.wParam;
}



//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MESSAGEHIDER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   OBJECTIF : enregistre le handle d'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        nous créons et affichons la fenêtre principale du programme.
//

HWND hWnd = nullptr;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Stocke le handle d'instance dans la variable globale

   float scale = 0.75;

   int screenWidth = GetSystemMetrics(SM_CXSCREEN) * scale;
   int screenHeight = GetSystemMetrics(SM_CYSCREEN) * scale;

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, screenWidth, screenHeight, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_COMPOSITED);

   ErrorHandler* errorHandler = new ErrorHandler(&hWnd);
   SetUnhandledExceptionFilter(errorHandler->UnhandledExceptionHandler);

   AppHandler::SetHWND(hWnd);

   EnableDarkModeForTitleBar(hWnd);
   ApplyTheme(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

bool isWriting = false;

void EnableRedraw(HWND hTextField, BOOL enable) {
    SendMessage(hTextField, WM_SETREDRAW, enable, 0);
    if (enable) {
        InvalidateRect(hTextField, NULL, TRUE); // Forcer un redessin complet
    }
}

// Fonction pour désactiver le redessin lorsque l'utilisateur commence à écrire
void OnKeyDown(HWND hFocused) {
    if (!isWriting) {
        isWriting = true;
        EnableRedraw(hFocused, FALSE); // Désactiver le redessin
    }
}

// Fonction pour réactiver le redessin lorsque l'utilisateur arrête d'écrire
void OnKeyUp(HWND hFocused) {
    if (isWriting) {
        isWriting = false;
        EnableRedraw(hFocused, TRUE); // Réactiver le redessin
    }
}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  OBJECTIF : Traite les messages pour la fenêtre principale.
//
//  WM_COMMAND  - traite le menu de l'application
//  WM_PAINT    - Dessine la fenêtre principale
//  WM_DESTROY  - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Filter* filter = new Filter();
    static ImageHandler* imageHandler = new ImageHandler(filter);

    static Interface* uiInterface = new Interface(hWnd);

    switch (message)
    {
    case WM_CREATE:
        uiInterface->CreateInterface();
        break;
    case WM_KEYDOWN:
    {
        HWND hTextField = GetDlgItem(hWnd, 69);
        if (GetFocus() == hTextField) {  // Si le focus est sur le champ texte
            OnKeyDown(hTextField);            // Désactiver le redessin
        }

        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            switch (wParam)
            {
            case 'S':  // Ctrl + S
                AppHandler::SaveImage();
                break;
            case 'O':  // Ctrl + O
                AppHandler::OpenImage();
                break;
            case 'E':  // Ctrl + E
                imageHandler->Write();
                break;
            case 'D':  // Ctrl + D
                imageHandler->Read();
                break;
            }
        }
        break;
    }
    case WM_KEYUP: {
        HWND hTextField = GetDlgItem(hWnd, 69);
        if (GetFocus() == hTextField) {  // Si le focus est sur le champ texte
            OnKeyUp(hTextField);              // Réactiver le redessin
        }
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        if (HIWORD(wParam) == EN_CHANGE && wmId == 69) {
            HWND hTextField = GetDlgItem(hWnd, 69);
            SendMessage(hTextField, WM_SETREDRAW, FALSE, 0);

            SendMessage(hTextField, WM_SETREDRAW, TRUE, 0);
        }

        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            SetFocus(hWnd);
            break;
        case IDM_OPEN_FILE:
            AppHandler::OpenImage();
            SetFocus(hWnd);
            break;
        case IDM_SAVE_FILE:
            AppHandler::SaveImage();
            SetFocus(hWnd);
            break;
        case IDM_HIDE_MESSAGE:
            imageHandler->Write();
            SetFocus(hWnd);
            break;
        case IDM_DECODE_MESSAGE:
            imageHandler->Read();
            SetFocus(hWnd);
            break;
        case IDM_SHOW_LOGS:
            ErrorHandler::GetInstance()->OpenLogWindow((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
            break;
        case ID_APPLY_BLACKWHITE_FILTER:
            imageHandler->ApplyFilter(Filter::BLACKWHITE_FILTER);
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        case ID_APPLY_INVERT_FILTER:
            imageHandler->ApplyFilter(Filter::INVERT_FILTER);
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        case ID_APPLY_BLUR_FILTER:
            imageHandler->ApplyFilter(Filter::BLUR_FILTER);
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        case ID_APPLY_SATURE_FILTER:
            imageHandler->ApplyFilter(Filter::SATURE_FILTER);
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        case ID_RESET_FILTER:
            imageHandler->Reset();
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        case IDM_ERROR_TEST:
        {
            ErrorHandler::GetInstance()->Error(ErrorHandler::ErrorType::ERROR_TEST);
            int* crashPtr = nullptr;
            *crashPtr = 42;
            break;
        }
        default:
            if (ID_VIEW_LIGHTMODE <= wmId <= ID_VIEW_BLUEMODE)
            {
                uiInterface->ChangeTheme(hWnd, wmId);
                ApplyTheme(hWnd);
                break;  
            }
                
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        uiInterface->HandlePaints(message);

        EndPaint(hWnd, &ps);

        break;
    }
    case WM_DRAWITEM:
    {
        uiInterface->ApplyTheme(lParam);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}