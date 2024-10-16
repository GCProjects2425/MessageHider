#pragma once
#include "framework.h"

class ErrorHandler
{
public:
	ErrorHandler(HWND* Hwnd) 
		: m_parentHwnd(Hwnd)
		, m_currentError((ErrorType)0x8000)
		, m_lastError((ErrorType)0x8000)
		, m_stateClick(0)
	{
		SetInstance(this);
	}

	~ErrorHandler()
	{
		delete m_Instance;
	}
	
	static ErrorHandler* GetInstance() { return m_Instance; }

	enum ErrorType
	{
		FILE_NOT_FOUND = 0,
		FILE_FORMAT_INCORRECT,
		NO_IMAGE_LOADED,
		TEXT_IS_TOO_LONG,

		ERROR_TEST,
		ERROR_COUNT
	};

	void Error(ErrorType errorType);

	bool IsRetryClicked() const { return m_stateClick == IDRETRY; };
	bool IsCancelClicked() const { return m_stateClick == IDCANCEL; };

	void TerminateHandler()
	{
		LogError("Unhandled error captured!");
		std::abort();
	}
	static LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* exceptionInfo);
	LONG UnhandledExceptionHandlerInstance(EXCEPTION_POINTERS* exceptionInfo);

private:
	static ErrorHandler* m_Instance;
	static const char* const m_ErrorList[];

	HWND* m_parentHwnd;
	
	ErrorType m_currentError;
	ErrorType m_lastError;

	int m_stateClick;

	void LogError(const std::string& errorMessage);

	static void SetInstance(ErrorHandler* errorHandler) { m_Instance = errorHandler; }

	void TriggerError();

};

