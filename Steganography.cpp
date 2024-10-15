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
