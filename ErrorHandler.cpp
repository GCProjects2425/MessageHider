#include "ErrorHandler.h"
#include <iostream>
#include <exception>
#include <fstream>
#include <cstdlib>
#include <format>
#include <chrono>
#include <sstream>
#include <filesystem>

const char* const ErrorHandler::m_ErrorList[ERROR_COUNT] = { 
	"File is missing.",
	"Incorrect file format.",
    "No image loaded.",
    "Text too long.",
    "Image export has failed.",
    "Ceci est un test d'erreur.",
};

void ErrorHandler::Error(ErrorType errorType)
{
	m_lastError = m_currentError;
	m_currentError = errorType;

	TriggerError();
}

const std::wstring& ErrorHandler::GetLogPath()
{
    std::filesystem::path cwd = std::filesystem::current_path() / "log.txt";
    return cwd.wstring();
}

LONG ErrorHandler::UnhandledExceptionHandlerInstance(EXCEPTION_POINTERS* exceptionInfo) {
    LogError("Critical error: unhandled error has triggered.");

    // Vous pouvez également afficher un message d'erreur ou un dialogue ici
    MessageBox(NULL, L"Critical error: The program has encountered a problem and will close.", L"Error", MB_OK);

    // Terminer le programme
    return EXCEPTION_EXECUTE_HANDLER;
}

void ErrorHandler::OpenLogWindow(HINSTANCE hInstance)
{
    const wchar_t LOG_CLASS_NAME[] = L"LogWindow";

    WNDCLASS wc = { };
    wc.lpfnWndProc = LogWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = LOG_CLASS_NAME;

    RegisterClass(&wc);

    HWND hwndLog = CreateWindowEx(0, LOG_CLASS_NAME, L"Log Viewer", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
        nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hwndLog, SW_SHOW);
}

LONG WINAPI ErrorHandler::UnhandledExceptionHandler(EXCEPTION_POINTERS* exceptionInfo)
{
    // Appeler la méthode d'instance à partir de la méthode statique
    if (m_Instance) {
        return m_Instance->UnhandledExceptionHandlerInstance(exceptionInfo);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

void ErrorHandler::LogError(const std::string& errorMessage)
{
    std::ofstream logFile(GetLogPath(), std::ios_base::app);
    auto now = std::chrono::system_clock::now();
    std::string formatted_time = std::format("[{0:%F_%T}]", now);
    if (logFile.is_open()) {
        logFile << "[ERROR]" << formatted_time << " : " << errorMessage << std::endl;
        logFile.close();
    }
}

void ErrorHandler::TriggerError()
{
    if (m_currentError >= ERROR_COUNT)
        return;

    const char* errorText = m_ErrorList[(int)m_currentError];
    LogError(errorText);
    int msgBox = MessageBoxA(
        NULL,
        errorText,
        "Error",
        MB_ICONERROR | MB_DEFBUTTON1 | MB_TASKMODAL
    );

    m_stateClick = msgBox;
}

LRESULT ErrorHandler::LogWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit;

    switch (uMsg) {
    case WM_CREATE:
        // Créer un champ de texte non modifiable avec une barre de défilement
        hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL,
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            10, 10, 460, 340, hWnd, (HMENU)1, GetModuleHandle(NULL), NULL);

        // Charger le fichier texte dans le champ de texte
        m_Instance->LoadFileContentIntoTextField(hEdit, GetLogPath().c_str());
        return 0;

    case WM_SIZE:
        // Redimensionner le champ de texte lorsqu'on redimensionne la fenêtre
        MoveWindow(hEdit, 10, 10, LOWORD(lParam) - 20, HIWORD(lParam) - 20, TRUE);
        return 0;

    case WM_DESTROY:
        //PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void ErrorHandler::LoadFileContentIntoTextField(HWND hEdit, const wchar_t* filename)
{
    std::wifstream file(filename);
    if (file.is_open()) {
        std::wstringstream buffer;
        buffer << file.rdbuf(); // Lire tout le contenu du fichier
        std::wstring wLogs = buffer.str();

        // Remplacer les caractères de retour à la ligne
        size_t pos = 0;
        while ((pos = wLogs.find('\n', pos)) != std::wstring::npos) {
            wLogs.replace(pos, 1, L"\r\n"); // Remplace les '\n' par '\r\n'
            pos += 2; // Avancer de 2 caractères pour éviter de boucler sur le même '\r'
        }

        // Mettre à jour le contenu du champ de texte
        SetWindowText(hEdit, wLogs.c_str());
    }
    else {
        SetWindowText(hEdit, L"Error while reading log file.");
    }
}



ErrorHandler* ErrorHandler::m_Instance = nullptr;