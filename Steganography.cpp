#include <vector>

#include "Steganography.h"

std::vector<int> HammingEncode(const std::vector<int>& messageBits) {
	std::vector<int> encodedBits(7);


	encodedBits[0] = messageBits[0];
	encodedBits[1] = messageBits[1];
	encodedBits[2] = messageBits[2];
	encodedBits[3] = (messageBits[0] ^ messageBits[1] ^ messageBits[2]);
	encodedBits[4] = messageBits[3];
	encodedBits[5] = (messageBits[0] ^ messageBits[1] ^ messageBits[3]);
	encodedBits[6] = (messageBits[0] ^ messageBits[2] ^ messageBits[3]);

	return encodedBits;
}
void Steganography::HideMessage(Gdiplus::Bitmap* bitmap, const std::string& text) {
	/*int width = bitmap->GetWidth();*/
	//int height = bitmap->GetHeight();
	//int messageLength = text.size();

	//int bitIndex = 0;
	//for (int i = 0; i < height; ++i) {
	//	for (int j = 0; j < width; ++j) {
	//		Color pixelColor;
	//		bitmap->GetPixel(j, i, &pixelColor);

	//		// Obtenir le caract�re � encoder
	//		if (bitIndex < messageLength * 8) {
	//			char currentChar = text[bitIndex / 8];
	//			// Encoder le bit dans le canal bleu
	//			int bitToEncode = (currentChar >> (7 - (bitIndex % 8))) & 1;
	//			int newBlue = (pixelColor.GetB() & ~1) | bitToEncode; // Modifier le bit de poids faible du canal bleu
	//			pixelColor = Color(pixelColor.GetA(), pixelColor.GetR(), pixelColor.GetG(), newBlue);
	//			bitmap->SetPixel(j, i, pixelColor);
	//			bitIndex++;
	//		}
	//		else {
	//			// Si le message est compl�tement encod�, arr�ter
	//			return;
	//		}
	//	}
	//}
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

	// Ajouter le caract�re de fin '\0' au message
	for (int i = 7; i >= 0; --i) {
		messageBits.push_back(0);  // '\0' en bits
	}

	// Encoder les bits du message avec Hamming (7,4)
	std::vector<int> encodedMessageBits;
	for (size_t i = 0; i < messageBits.size(); i += 4) {
		std::vector<int> chunk(4, 0);
		for (size_t j = 0; j < 4 && (i + j) < messageBits.size(); ++j) {
			chunk[j] = messageBits[i + j];
		}
		std::vector<int> encodedChunk = HammingEncode(chunk);
		encodedMessageBits.insert(encodedMessageBits.end(), encodedChunk.begin(), encodedChunk.end());
	}

	// Cacher les bits encod�s dans l'image
	for (int i = 0; i < height && bitIndex < encodedMessageBits.size(); ++i) {
		for (int j = 0; j < width && bitIndex < encodedMessageBits.size(); ++j) {
			Color pixelColor;
			bitmap->GetPixel(j, i, &pixelColor);

			// Modifier le bit de poids faible du canal bleu
			int newBlue = (pixelColor.GetB() & ~1) | encodedMessageBits[bitIndex];
			pixelColor = Color(pixelColor.GetA(), pixelColor.GetR(), pixelColor.GetG(), newBlue);
			bitmap->SetPixel(j, i, pixelColor);

			bitIndex++;
		}
	}
}
std::vector<int> HammingDecode(const std::vector<int>& encodedBits) {
	std::vector<int> messageBits(4);

	// D�codage des 4 bits de message
	messageBits[0] = encodedBits[0];
	messageBits[1] = encodedBits[1];
	messageBits[2] = encodedBits[2];
	messageBits[3] = encodedBits[4];

	return messageBits;
}

std::string Steganography::ExtractMessage(Gdiplus::Bitmap* bitmap)
{
	//std::string message;
	//int width = bitmap->GetWidth();
	//int height = bitmap->GetHeight();

	//int bitIndex = 0;
	//char currentChar = 0;
	//for (int i = 0; i < height; ++i) {
	//	for (int j = 0; j < width; ++j) {
	//		Color pixelColor;
	//		bitmap->GetPixel(j, i, &pixelColor);

	//		// Lire le bit de poids faible du canal bleu 
	//		int bitValue = pixelColor.GetB() & 1;  // Lire le bit du canal bleu
	//		currentChar = (currentChar << 1) | bitValue;
	//		bitIndex++;

	//		// Ajouter le caract�re � la cha�ne de message lorsque 8 bits ont �t� lus
	//		if (bitIndex == 8) {
	//			// V�rifier si c'est la fin du message
	//			if (currentChar == '\0') {
	//				return message;  // Arr�ter la lecture lorsqu'on rencontre '\0'
	//			}
	//			message += currentChar;
	//			currentChar = 0;  // R�initialiser pour le prochain caract�re
	//			bitIndex = 0;  // R�initialiser l'index des bits
	//		}
	//	}
	//}
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();
	int bitIndex = 0;
	std::vector<int> encodedMessageBits;

	// Extraire les bits encod�s de l'image
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			Color pixelColor;
			bitmap->GetPixel(j, i, &pixelColor);

			// Lire le bit de poids faible du canal bleu
			int bitValue = pixelColor.GetB() & 1;
			encodedMessageBits.push_back(bitValue);
		}
	}

	// D�coder les bits encod�s avec Hamming (7,4)
	std::vector<int> messageBits;
	for (size_t i = 0; i + 6 < encodedMessageBits.size(); i += 7) {
		std::vector<int> chunk(7);
		for (size_t j = 0; j < 7; ++j) {
			chunk[j] = encodedMessageBits[i + j];
		}
		std::vector<int> decodedChunk = HammingDecode(chunk);
		messageBits.insert(messageBits.end(), decodedChunk.begin(), decodedChunk.end());
	}

	// Convertir les bits en texte
	std::string message;
	for (size_t i = 0; i + 7 < messageBits.size(); i += 8) {
		char currentChar = 0;
		for (size_t j = 0; j < 8; ++j) {
			currentChar = (currentChar << 1) | messageBits[i + j];
		}

		// Arr�ter si le caract�re est '\0'
		if (currentChar == '\0') {
			break;
		}

		message += currentChar;
	}

	return message;
}
