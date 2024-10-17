#include <cstdlib>
#include "ImageHandler.h"
#include "AppHandler.h"
#include "ErrorHandler.h"
#include "Filter.h"

using namespace std;
#include"Steganography.h"
void ImageHandler::DestroyImage()
{
	delete m_Image;  // Supprimer l'image pr�c�dente si elle existe
	delete m_Bitmap;
}

bool ImageHandler::Load(const wchar_t* filePath){
    DestroyImage();
    m_Image = new Image(filePath);
	m_Bitmap = ToBitmap();

	HWND hStatic = GetDlgItem(AppHandler::GetHWND(), 55);

	// Récupérer les informations de l'image
	int width = m_Image->GetWidth();
	int height = m_Image->GetHeight();

	// Convertir les informations en une chaîne
	std::wstring info;
	info += L"Resolution : " + std::to_wstring(width) + L"x" + std::to_wstring(height) + L" | MIME Type : " + GetMimeType();
	SetWindowText(hStatic, info.c_str());

	// Copie de l'image pour la sauvegarder en cas de futur filtres
	m_BitmapFiltered = m_Bitmap->Clone(0, 0, m_Bitmap->GetWidth(), m_Bitmap->GetHeight(), m_Bitmap->GetPixelFormat());

    return isValidImage();
}

void ImageHandler::Draw(HDC hdc, int x, int y, int width, int height) {
	if (isValidImage()) 
	{
		Graphics graphics(hdc);
		graphics.DrawImage(m_Image, x, y, width, height); 
	}
	else
	{
		ErrorHandler::GetInstance()->Error(ErrorHandler::NO_IMAGE_LOADED);
	}
}


void ImageHandler::Write()
{
	if (isValidImage())
	{
		HWND hTextField = GetDlgItem(AppHandler::GetHWND(), 69);
		int imageLength = m_Image->GetHeight() * m_Image->GetWidth();
		int textLength = GetWindowTextLength(hTextField) + 1;
		if (textLength > imageLength)
		{
			ErrorHandler::GetInstance()->Error(ErrorHandler::TEXT_IS_TOO_LONG);
			return;
		}
		wchar_t* buffer = new wchar_t[imageLength];
		GetWindowText(hTextField, buffer, textLength);
		//GetDlgItemText(AppHandler::GetHWND(), 69, buffer, imageLength);

		WriteTextInBitmap(m_Bitmap, WCharToString(buffer));

		SetWindowText(hTextField, L"");

		MessageBoxA(AppHandler::GetHWND(), "The text has been correctly encrypted.", "Success!", 0);
	}
	else
	{
		ErrorHandler::GetInstance()->Error(ErrorHandler::NO_IMAGE_LOADED);
	}
}

void ImageHandler::Save(const wchar_t* filePath)
{
	if (isValidImage())
	{
		if (m_Bitmap == nullptr)
		{
			return;
		}

		CLSID imageClsid;
		wstring fileExtension = filePath;

		try
		{
			// Détecte l'extension du fichier pour choisir l'encodeur approprié
			if (fileExtension.find(L".jpg") != wstring::npos || fileExtension.find(L".jpeg") != wstring::npos)
			{
				if (GetEncoderClsid(L"image/jpeg", &imageClsid) != -1) 
				{
					m_Bitmap->Save(filePath, &imageClsid, NULL);
				}
			}
			else if (fileExtension.find(L".bmp") != wstring::npos)
			{
				if (GetEncoderClsid(L"image/bmp", &imageClsid) != -1) 
				{
					m_Bitmap->Save(filePath, &imageClsid, NULL);
				}
			}
			else if (fileExtension.find(L".gif") != wstring::npos)
			{
				if (GetEncoderClsid(L"image/gif", &imageClsid) != -1)  
				{
					m_Bitmap->Save(filePath, &imageClsid, NULL);
				}
			}
			else
			{
				if (GetEncoderClsid(L"image/png", &imageClsid) != -1) 
				{
					m_Bitmap->Save(filePath, &imageClsid, NULL);
				}
			}

			MessageBoxA(AppHandler::GetHWND(), "The image has been correctly exported.", "Success!", 0);
		}
		catch (const std::exception&)
		{
			ErrorHandler::GetInstance()->Error(ErrorHandler::EXPORT_FAILED);
		}
	}
	else
	{
		ErrorHandler::GetInstance()->Error(ErrorHandler::NO_IMAGE_LOADED);
	}
}

const wchar_t* ImageHandler::GetMimeType()
{
	GUID format;
	m_Image->GetRawFormat(&format);

	if (format == Gdiplus::ImageFormatBMP) {
		return L"image/bmp";
	}
	else if (format == Gdiplus::ImageFormatJPEG) {
		return L"image/jpeg";
	}
	else if (format == Gdiplus::ImageFormatPNG) {
		return L"image/png";
	}
	else if (format == Gdiplus::ImageFormatGIF) {
		return L"image/gif";
	}
	else if (format == Gdiplus::ImageFormatTIFF) {
		return L"image/tiff";
	}
	else {
		return L"unknown";
	}
}

std::string ImageHandler::Read()
{
	std::string textEncoded = "";
	if (isValidImage())
	{
		textEncoded = ReadTextInBitmap(m_Bitmap);
		HWND hTextField = GetDlgItem(AppHandler::GetHWND(), 69);
		SetWindowText(hTextField, ConvertToWideString(textEncoded).c_str());
		//MessageBoxA(AppHandler::GetHWND(), textEncoded.c_str(), "Test", 0);
	}
	else
	{
		ErrorHandler::GetInstance()->Error(ErrorHandler::NO_IMAGE_LOADED);
	}

	return textEncoded;
}

bool ImageHandler::isValidImage()
{
	return (m_Image != nullptr && m_Image->GetLastStatus() == Status::Ok);
}

void ImageHandler::ApplyFilter(Filter::Filters filter) 
{
	if (isValidImage()) {

		HCURSOR hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
		m_Filter->Apply(m_BitmapFiltered, filter);
		m_Image = static_cast<Image*>(m_BitmapFiltered);
		SetCursor(hOldCursor);
	}
	else
	{
		ErrorHandler::GetInstance()->Error(ErrorHandler::NO_IMAGE_LOADED);
	}
}

void ImageHandler::Reset()
{
	m_Image = static_cast<Image*>(m_Bitmap);
	m_BitmapFiltered = m_Bitmap->Clone(0, 0, m_Bitmap->GetWidth(), m_Bitmap->GetHeight(), m_Bitmap->GetPixelFormat());
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

std::wstring ImageHandler::ConvertToWideString(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
	return wstr;
}

ImageHandler* ImageHandler::m_Instance = nullptr;