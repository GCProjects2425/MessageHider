#include "Steganography.h"
void Steganography::HideMessage(Gdiplus::Bitmap* bitmap, const std::string& text) {
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();
	int messageLength = text.size();

	int bitIndex = 0;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			Color pixelColor;
			bitmap->GetPixel(j, i, &pixelColor);

			// Obtenir le caract�re � encoder
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
				// Si le message est compl�tement encod�, arr�ter
				return;
			}
		}
	}
}

std::string Steganography::ExtractMessage(Gdiplus::Bitmap* bitmap)
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

			// Ajouter le caract�re � la cha�ne de message lorsque 8 bits ont �t� lus
			if (bitIndex == 8) {
				// V�rifier si c'est la fin du message
				if (currentChar == '\0') {
					return message;  // Arr�ter la lecture lorsqu'on rencontre '\0'
				}
				message += currentChar;
				currentChar = 0;  // R�initialiser pour le prochain caract�re
				bitIndex = 0;  // R�initialiser l'index des bits
			}
		}
	}

}
