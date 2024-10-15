#pragma once
#include <string>
#include "framework.h"


class Steganography {
public:
    // Fonction pour cacher un message dans une image
    static void HideMessage(Gdiplus::Bitmap* bitmap, const std::string& message);

    // Fonction pour extraire un message d'une image
    static std::string ExtractMessage(Gdiplus::Bitmap* bitmap);
};