#include "BlackWhiteFilter.h"

void BlackWhiteFilter::Apply(Bitmap& image)
{
	for (int x = 0; x < image.GetWidth(); ++x) {
		for (int y = 0; y < image.GetHeight(); ++y) {
			Color pixel;
			image.GetPixel(x, y, &pixel);
			// Conversion en niveaux de gris : moyenne des valeurs RGB
			int gray = (pixel.Red + pixel.Green + pixel.Blue) / 3;
			Color grayColor(gray, gray, gray);
			image.SetPixel(x, y, grayColor);
		}
	}
}
