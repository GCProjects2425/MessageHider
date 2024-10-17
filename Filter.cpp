#include "Filter.h"

void Filter::Apply(Bitmap* bitmap, Filters filter)
{
    switch (filter)
    {
    case Filter::BLACKWHITE_FILTER:
        BlackWhiteFilter(bitmap);
        break;
    case Filter::INVERT_FILTER:
        break;
    case Filter::BLUR_FILTER:
        break;
    case Filter::SATURE_FILTER:
        break;
    default:
        break;
    }
}

void Filter::BlackWhiteFilter(Bitmap* image)
{
    if (!image) return;

    // Verrouiller les bits de l'image pour la lecture/écriture
    BitmapData bitmapData;
    Rect rect(0, 0, image->GetWidth(), image->GetHeight());
    image->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);

    // Pointeur sur les pixels de l'image
    BYTE* pixels = static_cast<BYTE*>(bitmapData.Scan0);

    // Parcourir chaque pixel
    for (UINT y = 0; y < image->GetHeight(); ++y)
    {
        for (UINT x = 0; x < image->GetWidth(); ++x)
        {
            BYTE* pixel = pixels + y * bitmapData.Stride + x * 4;  // Chaque pixel a 4 octets (ARGB)

            BYTE blue = pixel[0];  // Bleu
            BYTE green = pixel[1]; // Vert
            BYTE red = pixel[2];   // Rouge

            // Calcul de la luminosité (formule standard pour convertir en niveaux de gris)
            BYTE gray = static_cast<BYTE>(0.299 * red + 0.587 * green + 0.114 * blue);

            // Remplacer les valeurs R, G, B par la valeur de gris
            pixel[0] = gray;  // Bleu
            pixel[1] = gray;  // Vert
            pixel[2] = gray;  // Rouge
        }
    }

    // Déverrouiller les bits après modification
    image->UnlockBits(&bitmapData);
}
