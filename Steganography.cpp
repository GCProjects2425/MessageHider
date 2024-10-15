#include "Steganography.h"
void Steganography::HideMessage(Gdiplus::Bitmap* bitmap, const std::string& message) {
    Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
    BitmapData bitmapData;

    // Verrouiller les bits de l'image pour modification
    bitmap->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

    UINT* pixels = (UINT*)bitmapData.Scan0;
    int width = bitmap->GetWidth();
    int height = bitmap->GetHeight();
    int pixelCount = width * height;

    int charIndex = 0;
    int bitIndex = 0;
    char currentChar = message[charIndex];

    for (int i = 0; i < pixelCount && charIndex < message.size(); ++i) {
        UINT* pixel = pixels + i;

        for (int colorIndex = 0; colorIndex < 3; ++colorIndex) {
            BYTE* colorComponent = ((BYTE*)pixel) + colorIndex;

            *colorComponent &= 0xFC; // Supprimer les 2 derniers bits du composant
            *colorComponent |= ((currentChar >> (bitIndex * 2)) & 0x03); // Insérer les bits du message

            bitIndex++;

            if (bitIndex == 4) { // Si on a caché tous les bits d'un caractère
                bitIndex = 0;
                charIndex++;
                if (charIndex < message.size()) {
                    currentChar = message[charIndex]; // Passer au caractère suivant
                }
                else {
                    currentChar = '\0'; // Indiquer la fin du texte
                }
            }
        }
    }

    // Déverrouiller les bits après modification
    bitmap->UnlockBits(&bitmapData);
}
