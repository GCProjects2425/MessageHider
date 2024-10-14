#pragma once
#include "framework.h"

class ImageHandler
{
public:
	ImageHandler()
		: m_Image(nullptr)
	{};
	
	~ImageHandler()
	{
		DestroyImage();
	}

	void DestroyImage();

	bool Load(const wchar_t* filePath);
	void Draw(HDC hdc, int x, int y);

	void Write();
	std::string Read();

	bool isValidImage();

private:
	Image* m_Image;

	Bitmap* ToBitmap();
	void WriteTextInBitmap(Bitmap* bitmap, const std::string& text);
	std::string ReadTextInBitmap(Bitmap* bitmap);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
};

