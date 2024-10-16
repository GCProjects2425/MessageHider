#pragma once
#include "framework.h"

class ImageHandler
{
public:
	ImageHandler()
		: m_Image(nullptr)
	{
		SetInstance(this);
	};
	
	~ImageHandler()
	{
		delete m_Instance;
		DestroyImage();
	}

	void DestroyImage();

	bool Load(const wchar_t* filePath);
	void Draw(HDC hdc, int x, int y, int width, int height);

	std::string Read();
	void Write();
	
	void Save(const wchar_t* filePath);

	bool isValidImage();

	static ImageHandler* GetInstance() { return m_Instance; };

private:
	Image* m_Image;

	static void SetInstance(ImageHandler* imageHandler) { m_Instance = imageHandler; }

	static ImageHandler* m_Instance;

	Bitmap* ToBitmap();
	void WriteTextInBitmap(Bitmap* bitmap, const std::string& text);
	std::string ReadTextInBitmap(Bitmap* bitmap);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	std::string WCharToString(const wchar_t* wstr);
};

