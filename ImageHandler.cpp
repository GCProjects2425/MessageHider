#include "ImageHandler.h"

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
		WriteTextInBitmap(bitmap, "salut ceci est un texte");

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
	Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
	BitmapData bitmapData;

	bitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

	UINT* pixels = (UINT*)bitmapData.Scan0;
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();
	int pixelCount = width * height;

	int charIndex = 0;
	int bitIndex = 0;
	char currentChar = text[charIndex];

	for (int i = 0; i < pixelCount && charIndex < text.size(); ++i)
	{
		UINT* pixel = pixels + i;

		for (int colorIndex = 0; colorIndex < 3; ++colorIndex)
		{
			BYTE* colorComponent = ((BYTE*)pixel) + colorIndex;

			*colorComponent &= 0xFC; // supprimer les 2 derniers bits
			*colorComponent |= ((currentChar >> (bitIndex * 2)) & 0x03); // remplacer les 2 derniers bits par les bits du char

			bitIndex++;

			if (bitIndex == 4)
			{
				bitIndex = 0;
				charIndex++;
				if (charIndex < text.size())
				{
					currentChar = text[charIndex]; // on passe au char suivant
				}
				else
				{
					currentChar = '\0'; // si on est a la fin du texte
				}
			}
		}
	}

	bitmap->UnlockBits(&bitmapData);
}

std::string ImageHandler::ReadTextInBitmap(Bitmap* bitmap)
{
	Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
	BitmapData bitmapData;

	bitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

	UINT* pixels = (UINT*)bitmapData.Scan0;
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();
	int pixelCount = width * height;

	std::string decodedText;
	char currentChar = 0;
	int bitIndex = 0;

	for (int i = 0; 0 < pixelCount; ++i)
	{
		UINT* pixel = pixels + i;

		for (int colorIndex = 0; colorIndex < 3; ++colorIndex)
		{
			BYTE* colorComponent = ((BYTE*)pixel) + colorIndex;

			currentChar |= (*colorComponent & 0x03) << (bitIndex * 2); // Lire les 2 derniers bits

			bitIndex++;
			if (bitIndex == 4)
			{
				if (currentChar == '\0')  // Si c'est la fin du texte
					break;
				decodedText += currentChar;
				currentChar = 0;
				bitIndex = 0;
			}
		}
	}

	bitmap->UnlockBits(&bitmapData);
	return decodedText;
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