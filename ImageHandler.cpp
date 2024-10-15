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
		WriteTextInBitmap(bitmap, "Leit Leith");

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
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();
	int messageLength = text.size();

	int bitIndex = 0;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			Color pixelColor;
			bitmap->GetPixel(j, i, &pixelColor);

			// Obtenir le caractère à encoder
			if (bitIndex < messageLength * 8) {
				char currentChar = text[bitIndex / 8];
				// Encoder le bit dans le canal bleu
				int bitToEncode = (currentChar >> (7 - (bitIndex % 8))) & 1;
				int newBlue = (pixelColor.GetB() & ~1) | bitToEncode; // Modifier le bit de poids faible du canal bleu
				pixelColor = Color(pixelColor.GetA(), pixelColor.GetR(), pixelColor.GetG(), newBlue);
				bitmap->SetPixel(j, i, pixelColor);
				bitIndex++;
			}
			else {
				// Si le message est complètement encodé, arrêter
				return;
			}
		}
	}
}

std::string ImageHandler::ReadTextInBitmap(Bitmap* bitmap)
{
	std::string message;
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();

	int bitIndex = 0;
	char currentChar = 0;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			Color pixelColor;
			bitmap->GetPixel(j, i, &pixelColor);

			// Lire le bit de poids faible du canal bleu 
			int bitValue = pixelColor.GetB() & 1;  // Lire le bit du canal bleu
			currentChar = (currentChar << 1) | bitValue;
			bitIndex++;

			// Ajouter le caractère à la chaîne de message lorsque 8 bits ont été lus
			if (bitIndex == 8) {
				// Vérifier si c'est la fin du message
				if (currentChar == '\0') {
					return message;  // Arrêter la lecture lorsqu'on rencontre '\0'
				}
				message += currentChar;
				currentChar = 0;  // Réinitialiser pour le prochain caractère
				bitIndex = 0;  // Réinitialiser l'index des bits
			}
		}
	}

	
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