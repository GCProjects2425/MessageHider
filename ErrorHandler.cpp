#include "ErrorHandler.h"
#include <iostream>
#include <exception>
#include <fstream>
#include <cstdlib>
#include <format>
#include <chrono>

const char* const ErrorHandler::m_ErrorList[ERROR_COUNT] = { 
	"Le fichier est introuvable",
	"Le format de fichier est incorrect",
    "Aucune image n'est chargée",
    "Le texte est trop long",
    "Ceci est un test d'erreur",
};

void ErrorHandler::Error(ErrorType errorType)
{
	m_lastError = m_currentError;
	m_currentError = errorType;

	TriggerError();
}

LONG ErrorHandler::UnhandledExceptionHandlerInstance(EXCEPTION_POINTERS* exceptionInfo) {
    LogError("Critical error: unhandled error has triggered.");

    // Vous pouvez également afficher un message d'erreur ou un dialogue ici
    MessageBox(NULL, L"Erreur critique: Le programme a rencontré un problème et va se fermer.", L"Erreur", MB_OK);

    // Terminer le programme
    return EXCEPTION_EXECUTE_HANDLER;
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
    std::ofstream logFile("log.txt", std::ios_base::app);
    auto now = std::chrono::system_clock::now();
    std::string formatted_time = std::format("{0:%F_%T}", now);
    if (logFile.is_open()) {
        logFile << "[ERROR] " << formatted_time << " : " << errorMessage << std::endl;
        logFile.close();
    }
}

void ErrorHandler::TriggerError()
{
    if (m_currentError >= ERROR_COUNT)
        return;

    const char* errorText = m_ErrorList[(int)m_currentError];
    int msgBox = MessageBoxA(
        NULL,
        errorText,
        "Une erreur est survenue",
        MB_ICONERROR | MB_RETRYCANCEL | MB_DEFBUTTON2
    );

    /*if (msgBox == IDYES)
    {
        // L'utilisateur a choisi "Oui"
        MessageBox(*m_parentHwnd, L"Vous avez choisi Oui", L"Résultat", MB_OK);
    }
    else if (msgBox == IDNO)
    {
        // L'utilisateur a choisi "Non"
        MessageBox(*m_parentHwnd, L"Vous avez choisi Non", L"Résultat", MB_OK);
    }*/
}



ErrorHandler* ErrorHandler::m_Instance = nullptr;