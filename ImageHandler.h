#pragma once
#include "framework.h"
#include "Filter.h"

class ImageHandler
{
public:
	ImageHandler(Filter* filter)
		: m_Image(nullptr)
		, m_Bitmap(nullptr)
		, m_Filter(filter)
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

	void ApplyFilter(Filter::Filters filter);

private:
	Image* m_Image;
	Bitmap* m_Bitmap;
	Filter* m_Filter;

	static void SetInstance(ImageHandler* imageHandler) { m_Instance = imageHandler; }

	static ImageHandler* m_Instance;

	Bitmap* ToBitmap();
	void WriteTextInBitmap(Bitmap* bitmap, const std::string& text);
	std::string ReadTextInBitmap(Bitmap* bitmap);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	std::string WCharToString(const wchar_t* wstr);
	std::wstring ConvertToWideString(const std::string& str);
};

