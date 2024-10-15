#pragma once
#include "framework.h"
#include "ImageHandler.h"

class AppHandler
{
public:
	static void SetHWND(HWND hWnd) { m_hWnd = hWnd; }
	static void OpenImage();
	static void SaveImage();
	static HWND& GetHWND() { return m_hWnd; }

private:
	static HWND m_hWnd;
};

