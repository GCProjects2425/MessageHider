#include <vector>

#include "Steganography.h"


void Steganography::HideMessage(Gdiplus::Bitmap* bitmap, const std::string& text) {
	/*int width = bitmap->GetWidth();
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
	}*/
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();
	int bitIndex = 0;

	// Convertir le texte en bits
	std::vector<int> messageBits;
	for (char c : text) {
		for (int i = 7; i >= 0; --i) {
			messageBits.push_back((c >> i) & 1); // Extraire les bits un par un
		}
	}

	// Ajouter le caractère de fin '\0' au message
	for (int i = 7; i >= 0; --i) {
		messageBits.push_back(0);  // '\0' en bits
	}

	// Cacher les bits dans l'image en minimisant les modifications
	for (int i = 0; i < height && bitIndex < messageBits.size(); ++i) {
		for (int j = 0; j < width && bitIndex < messageBits.size(); ++j) {
			Color pixelColor;
			bitmap->GetPixel(j, i, &pixelColor);

			// Lire le bit de poids faible du canal bleu
			int currentBit = pixelColor.GetB() & 1;

			// Si le bit actuel est différent du bit du message, on le modifie
			if (currentBit != messageBits[bitIndex]) {
				int newBlue = (pixelColor.GetB() & ~1) | messageBits[bitIndex];
				pixelColor = Color(pixelColor.GetA(), pixelColor.GetR(), pixelColor.GetG(), newBlue);
				bitmap->SetPixel(j, i, pixelColor);
			}

			bitIndex++;
		}
	}
}


std::string Steganography::ExtractMessage(Gdiplus::Bitmap* bitmap)
{
	/*std::string message;
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
	}*/
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();
	int bitIndex = 0;
	std::vector<int> messageBits;

	// Extraire les bits cachés dans l'image
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			Color pixelColor;
			bitmap->GetPixel(j, i, &pixelColor);

			// Lire le bit de poids faible du canal bleu
			int bitValue = pixelColor.GetB() & 1;
			messageBits.push_back(bitValue);

			bitIndex++;

			// Arrêter si la longueur maximale de message est atteinte
			if (messageBits.size() % 8 == 0) {
				char currentChar = 0;
				for (int k = 0; k < 8; ++k) {
					currentChar = (currentChar << 1) | messageBits[bitIndex - 8 + k];
				}

				if (currentChar == '\0') {
					std::string message;
					for (size_t i = 0; i < messageBits.size() - 8; i += 8) {
						char c = 0;
						for (int j = 0; j < 8; ++j) {
							c = (c << 1) | messageBits[i + j];
						}
						message += c;
					}
					return message; // Retourner le message si on trouve le caractère de fin '\0'
				}
			}
		}
	}

	return "";
}
