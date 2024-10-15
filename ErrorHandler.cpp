#include "ErrorHandler.h"

const char* const ErrorHandler::m_ErrorList[ERROR_COUNT] = { 
	"Le fichier est introuvable",
	"Le format de fichier est incorrect"
};

void ErrorHandler::Error(ErrorType& errorType)
{
	m_lastError = m_currentError;
	m_currentError = errorType;

	TriggerError();
}

void ErrorHandler::TriggerError()
{
    const char* errorText = m_ErrorList[m_currentError];
    m_stateClick = MessageBox(
        *m_parentHwnd,
        (LPCWSTR)errorText,
        (LPCWSTR)L"Une erreur est survenue",
            MB_ICONERROR | MB_RETRYCANCEL | MB_DEFBUTTON2
    );
}
