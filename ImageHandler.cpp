#include <cstdlib>
#include "ImageHandler.h"
#include "AppHandler.h"

using namespace std;
#include"Steganography.h"
void ImageHandler::DestroyImage()
{
	delete m_Image;  // Supprimer l'image pr�c�dente si elle existe
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
		int imageLength = m_Image->GetHeight() * m_Image->GetWidth();
		wchar_t* buffer = new wchar_t[imageLength];
		GetDlgItemText(AppHandler::GetHWND(), 69, buffer, imageLength);

		Bitmap* bitmap = ToBitmap();
		WriteTextInBitmap(bitmap, WCharToString(buffer));

		/*CLSID pngClsid;
		GetEncoderClsid(L"image/png", &pngClsid);*/
	}
}

void ImageHandler::Save(const wchar_t* filePath)
{
	// Vérifie que l'image est valide avant de continuer
	if (isValidImage())
	{
		// Conversion de l'image en Bitmap GDI+
		Bitmap* bitmap = ToBitmap();

		if (bitmap == nullptr)
		{
			// Gestion d'erreur : échec de la conversion en Bitmap
			return;
		}

		CLSID imageClsid;
		wstring fileExtension = filePath;

		// Détecte l'extension du fichier pour choisir l'encodeur approprié
		if (fileExtension.find(L".jpg") != wstring::npos || fileExtension.find(L".jpeg") != wstring::npos)
		{
			if (GetEncoderClsid(L"image/jpeg", &imageClsid) != -1) 
			{
				bitmap->Save(filePath, &imageClsid, NULL);
			}
		}
		else if (fileExtension.find(L".bmp") != wstring::npos)
		{
			if (GetEncoderClsid(L"image/bmp", &imageClsid) != -1) 
			{
				bitmap->Save(filePath, &imageClsid, NULL);
			}
		}
		else if (fileExtension.find(L".gif") != wstring::npos)
		{
			if (GetEncoderClsid(L"image/gif", &imageClsid) != -1)  
			{
				bitmap->Save(filePath, &imageClsid, NULL);
			}
		}
		else
		{
			if (GetEncoderClsid(L"image/png", &imageClsid) != -1) 
			{
				bitmap->Save(filePath, &imageClsid, NULL);
			}
		}

		DestroyImage();
	}
}




std::string ImageHandler::Read()
{
	std::string textEncoded = "";
	if (isValidImage())
	{
		Bitmap* bitmap = ToBitmap();
		//textEncoded = ReadTextInBitmap(bitmap);
		
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

std::string ImageHandler::WCharToString(const wchar_t* wstr) {
	std::wstring ws(wstr);
	std::string str(ws.begin(), ws.end());
	return str;
}

ImageHandler* ImageHandler::m_Instance = nullptr;