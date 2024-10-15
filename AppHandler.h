#pragma once
#include "framework.h"
#include "ImageHandler.h"

class AppHandler
{
public:
	static void OpenImage(HWND& hWnd, ImageHandler& imageHandler);
	static void SaveImage(HWND& hWnd, ImageHandler& imageHandler);
};

