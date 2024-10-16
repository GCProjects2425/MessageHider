#include "ErrorHandler.h"

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