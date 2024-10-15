#include "ImageHandler.h"
#include"Steganography.h"
void ImageHandler::DestroyImage()
{
	delete m_Image;  // Supprimer l'image précédente si elle existe
}

bool ImageHandler::Load(const wchar_t* filePath){
    DestroyImage();
    m_Image = new Image(filePath);
    return isValidImage();
}

void ImageHandler::Draw(HDC hdc, int x, int y)
{
	if(m_Image)
	{
		Graphics graphics(hdc);
		graphics.DrawImage(m_Image, x, y);
	}
}

void ImageHandler::Write()
{
	if (isValidImage())
	{
		Bitmap* bitmap = ToBitmap();
		WriteTextInBitmap(bitmap, "Salut ceci est un text");

		CLSID pngClsid;
		GetEncoderClsid(L"image/png", &pngClsid);
		bitmap->Save(L"test.png", &pngClsid, NULL);
	}
}

std::string ImageHandler::Read()
{
	std::string textEncoded = "";
	if (isValidImage())
	{
		Bitmap* bitmap = ToBitmap();
		textEncoded = ReadTextInBitmap(bitmap);
		
	}

	return textEncoded;
}

bool ImageHandler::isValidImage()
{
	return (m_Image != nullptr && m_Image->GetLastStatus() == Status::Ok);
}

Bitmap* ImageHandler::ToBitmap()
{
	Bitmap* bitmap = dynamic_cast<Bitmap*>(m_Image);
	if (!bitmap)
	{
		bitmap = new Bitmap(m_Image->GetWidth(), m_Image->GetHeight(), PixelFormat32bppARGB);
		Graphics graphics(bitmap);
		graphics.DrawImage(m_Image, 0, 0, m_Image->GetWidth(), m_Image->GetHeight());
	}

	return bitmap;
}

void ImageHandler::WriteTextInBitmap(Bitmap* bitmap, const std::string& text)
{
	Steganography::HideMessage(bitmap, text);
}

std::string ImageHandler::ReadTextInBitmap(Bitmap* bitmap)
{
	
	return Steganography::ExtractMessage(bitmap);
}

int ImageHandler::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;
	UINT size = 0;

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0) return -1;

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL) return -1;

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}

	free(pImageCodecInfo);
	return -1;
}