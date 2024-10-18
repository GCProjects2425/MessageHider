#include "Filter.h"
#include "ImageHandler.h"
#include "framework.h"

void Filter::Apply(Bitmap* bitmap, Filters filter)
{
    const std::string& textEncoded = ImageHandler::GetInstance()->Read();
    switch (filter)
    {
    case Filter::BLACKWHITE_FILTER:
        BlackWhiteFilter(bitmap);
        break;
    case Filter::INVERT_FILTER:
        InvertFilter(bitmap);
        break;
    case Filter::BLUR_FILTER:
        BlurFilter(bitmap);
        break;
    case Filter::SATURE_FILTER:
        SatureFilter(bitmap);
        break;
    default:
        break;
    }
    ImageHandler::GetInstance()->WriteTextInBitmap(bitmap, textEncoded);
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

            BYTE blue = pixel[0]; 
            BYTE green = pixel[1];
            BYTE red = pixel[2];  

            // Calcul de la luminosité (formule standard pour convertir en niveaux de gris)
            BYTE gray = static_cast<BYTE>(0.299 * red + 0.587 * green + 0.114 * blue);

            // Remplacer les valeurs R, G, B par la valeur de gris
            pixel[0] = gray; 
            pixel[1] = gray; 
            pixel[2] = gray; 
        }
    }

    // Déverrouiller les bits après modification
    image->UnlockBits(&bitmapData);
}

void Filter::InvertFilter(Bitmap* image)
{
    if (!image) return;

    // Verrouiller les bits de l'image pour la lecture/écriture
    BitmapData bitmapData;
    Rect rect(0, 0, image->GetWidth(), image->GetHeight());

    // Bloquer les bits du bitmap pour les manipuler directement
    if (image->LockBits(&rect, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData) == Ok) {
        // Pointer vers le début des données d'image
        BYTE* pixels = static_cast<BYTE*>(bitmapData.Scan0);

        // Parcourir chaque ligne (chaque "scanline")
        for (UINT y = 0; y < image->GetHeight(); y++) {
            // Parcourir chaque pixel de la ligne
            BYTE* pixel = pixels + y * bitmapData.Stride; // Stride donne la largeur totale de la ligne, y compris le padding

            for (UINT x = 0; x < image->GetWidth(); x++) {
                // Inverser les bits de chaque composante de couleur (R, G, B)
                pixel[0] = 255 - pixel[0]; // Composante bleue
                pixel[1] = 255 - pixel[1]; // Composante verte
                pixel[2] = 255 - pixel[2]; // Composante rouge
                // pixel[3] : Composante alpha (si nécessaire, on peut l'ignorer ou la traiter différemment)

                // Passer au pixel suivant (chaque pixel a 4 bytes en 32bpp)
                pixel += 4;
            }
        }

        // Débloquer les bits
        image->UnlockBits(&bitmapData);
    }
}

void Filter::BlurFilter(Bitmap* image)
{
    if (!image) return;

    int width = image->GetWidth();
    int height = image->GetHeight();
    int radius = GetRadiusAsPercentage(image, 5);

    // Créer des buffers temporaires pour stocker les résultats intermédiaires
    BitmapData bitmapData;
    Rect rect(0, 0, width, height);

    if (image->LockBits(&rect, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData) == Ok) {
        BYTE* pixels = static_cast<BYTE*>(bitmapData.Scan0);
        int stride = bitmapData.Stride;

        // Première passe : Flou horizontal
        std::vector<BYTE> tempBuffer(height * stride);
        #pragma omp parallel for
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int avgR = 0, avgG = 0, avgB = 0, avgA = 0;
                int count = 0;

                // Parcourir les pixels voisins horizontalement
                for (int kx = -radius; kx <= radius; ++kx) {
                    int neighborX = std::min(std::max(x + kx, 0), width - 1);

                    BYTE* neighborPixel = pixels + (y * stride) + (neighborX * 4);
                    avgB += neighborPixel[0]; // Bleu
                    avgG += neighborPixel[1]; // Vert
                    avgR += neighborPixel[2]; // Rouge
                    avgA += neighborPixel[3]; // Alpha
                    ++count;
                }

                // Calculer la moyenne et stocker dans le buffer temporaire
                tempBuffer[(y * stride) + (x * 4) + 0] = avgB / count;
                tempBuffer[(y * stride) + (x * 4) + 1] = avgG / count;
                tempBuffer[(y * stride) + (x * 4) + 2] = avgR / count;
                tempBuffer[(y * stride) + (x * 4) + 3] = avgA / count;
            }
        }

        // Deuxième passe : Flou vertical
        #pragma omp parallel for
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int avgR = 0, avgG = 0, avgB = 0, avgA = 0;
                int count = 0;

                // Parcourir les pixels voisins verticalement
                for (int ky = -radius; ky <= radius; ++ky) {
                    int neighborY = std::min(std::max(y + ky, 0), height - 1);

                    BYTE* neighborPixel = &tempBuffer[(neighborY * stride) + (x * 4)];
                    avgB += neighborPixel[0];
                    avgG += neighborPixel[1];
                    avgR += neighborPixel[2];
                    avgA += neighborPixel[3];
                    ++count;
                }

                // Mettre à jour les pixels dans le bitmap
                BYTE* currentPixel = pixels + (y * stride) + (x * 4);
                currentPixel[0] = avgB / count;
                currentPixel[1] = avgG / count;
                currentPixel[2] = avgR / count;
                currentPixel[3] = avgA / count;
            }
        }

        image->UnlockBits(&bitmapData);
    }
}

void Filter::SatureFilter(Bitmap* image)
{
    if (!image) return;

    // Verrouiller les bits de l'image pour la lecture/écriture
    BitmapData bitmapData;
    Rect rect(0, 0, image->GetWidth(), image->GetHeight());
    if (image->LockBits(&rect, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData) == Ok)
    {
        // Pointer vers le début des données d'image
        BYTE* pixels = static_cast<BYTE*>(bitmapData.Scan0);
        int stride = bitmapData.Stride;

        float saturationLevel = 1.5f; // Niveau de saturation (1.0 = aucune modification, >1.0 = saturation accrue)

        // Parcourir chaque pixel de l'image
        for (UINT y = 0; y < image->GetHeight(); ++y)
        {
            for (UINT x = 0; x < image->GetWidth(); ++x)
            {
                BYTE* pixel = pixels + y * stride + x * 4;  // Chaque pixel a 4 octets (ARGB)

                BYTE blue = pixel[0];  // Bleu
                BYTE green = pixel[1]; // Vert
                BYTE red = pixel[2];   // Rouge

                // Calculer la luminosité moyenne (luminosité perçue)
                float gray = 0.299f * red + 0.587f * green + 0.114f * blue;

                // Appliquer la saturation en ajustant la distance par rapport à la moyenne
                pixel[0] = static_cast<BYTE>(std::min(255.0f, gray + (blue - gray) * saturationLevel));  // Bleu
                pixel[1] = static_cast<BYTE>(std::min(255.0f, gray + (green - gray) * saturationLevel)); // Vert
                pixel[2] = static_cast<BYTE>(std::min(255.0f, gray + (red - gray) * saturationLevel));   // Rouge
            }
        }

        // Déverrouiller les bits après modification
        image->UnlockBits(&bitmapData);
    }
}

int Filter::GetRadiusAsPercentage(Bitmap* image, float percentage)
{
    if (!image || percentage < 0 || percentage > 100) {
        return -1; // Gérer les erreurs, par exemple avec un retour invalide
    }

    // Récupérer les dimensions de l'image
    int width = image->GetWidth();
    int height = image->GetHeight();

    // Utiliser la dimension la plus petite (largeur ou hauteur) pour calculer le rayon
    int minDimension = std::min(width, height);

    // Calculer le rayon en fonction du pourcentage
    int radius = static_cast<int>((percentage / 100.0f) * minDimension);

    return radius;
}
