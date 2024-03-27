//
// Created by mathanaruban on 25/11/23.
//
/* Fonction voronoi et dithering pour le rendu d'image */

#include "ima.h"



int trouverIndiceCouleur(Pixel *clut, int clutSize, Pixel couleur) {
    for (int i = 0; i < clutSize; i++) {
        if (couleur.red == clut[i].red &&
            couleur.green == clut[i].green &&
            couleur.blue == clut[i].blue) {
            return i;
        }
    }
    return -1;
}

ClutValue creatClut(Image *im) {
    int nb_pixel = im->sizeY * im->sizeX;
    Pixel *clut = malloc(nb_pixel * sizeof(Pixel));
    assert(clut);

    Pixel *colors = malloc(nb_pixel * sizeof(Pixel));
    assert(colors);

    int *clutIndices = malloc(nb_pixel * sizeof(int));
    assert(clutIndices);

    int currentColor = 0;

    for (int i = 0; i < nb_pixel; i++) {
        colors[i].red = im->data[i * 3];
        colors[i].green = im->data[i * 3 + 1];
        colors[i].blue = im->data[i * 3 + 2];

        int indiceCouleur = -1;

        for (int j = 0; j < currentColor; j++) {
            if (colors[i].red == clut[j].red &&
                colors[i].green == clut[j].green &&
                colors[i].blue == clut[j].blue) {
                indiceCouleur = j;
                break;
            }
        }

        if (indiceCouleur == -1) {
            // Nouvelle couleur
            clut[currentColor] = colors[i];
            clutIndices[i] = currentColor;
            currentColor++;
        } else {
            // Couleur existante
            clutIndices[i] = indiceCouleur;
        }
    }

    ClutValue clutData;
    clutData.clut = clut;
    clutData.clutIndices = clutIndices;
    clutData.clutSize = currentColor;

    // Libérer la mémoire
    free(colors);

    return clutData;
}

void dithering(Image *im) {
    ClutValue clutData = creatClut(im);

    int width = im->sizeX;
    int height = im->sizeY;
    Pixel *clut = clutData.clut;
    int *clutIndices = clutData.clutIndices;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixelIndex = y * width + x;
            int oldPixelIndex = clutIndices[pixelIndex];
            Pixel oldPixel = clut[oldPixelIndex];
            Pixel newPixel = clut[clutIndices[pixelIndex]];

            // Mettre à jour le pixel avec l'erreur de quantification
            im->data[pixelIndex * 3] = newPixel.red;
            im->data[pixelIndex * 3 + 1] = newPixel.green;
            im->data[pixelIndex * 3 + 2] = newPixel.blue;

            // Calculer l'erreur de quantification
            int quantizationErrorRed = oldPixel.red - newPixel.red;
            int quantizationErrorGreen = oldPixel.green - newPixel.green;
            int quantizationErrorBlue = oldPixel.blue - newPixel.blue;

            // Diffuser l'erreur aux pixels voisins
            if (x < width - 1) {
                // Pixel à droite
                int rightPixelIndex = y * width + (x + 1);
                im->data[rightPixelIndex * 3] += quantizationErrorRed * 7 / 16;
                im->data[rightPixelIndex * 3 + 1] += quantizationErrorGreen * 7 / 16;
                im->data[rightPixelIndex * 3 + 2] += quantizationErrorBlue * 7 / 16;
            }

            if (x > 0 && y < height - 1) {
                // Pixel en bas à gauche
                int bottomLeftPixelIndex = (y + 1) * width + (x - 1);
                im->data[bottomLeftPixelIndex * 3] += quantizationErrorRed * 3 / 16;
                im->data[bottomLeftPixelIndex * 3 + 1] += quantizationErrorGreen * 3 / 16;
                im->data[bottomLeftPixelIndex * 3 + 2] += quantizationErrorBlue * 3 / 16;
            }

            if (y < height - 1) {
                // Pixel en bas
                int bottomPixelIndex = (y + 1) * width + x;
                im->data[bottomPixelIndex * 3] += quantizationErrorRed * 5 / 16;
                im->data[bottomPixelIndex * 3 + 1] += quantizationErrorGreen * 5 / 16;
                im->data[bottomPixelIndex * 3 + 2] += quantizationErrorBlue * 5 / 16;
            }

            if (x < width - 1 && y < height - 1) {
                // Pixel en bas à droite
                int bottomRightPixelIndex = (y + 1) * width + (x + 1);
                im->data[bottomRightPixelIndex * 3] += quantizationErrorRed * 1 / 16;
                im->data[bottomRightPixelIndex * 3 + 1] += quantizationErrorGreen * 1 / 16;
                im->data[bottomRightPixelIndex * 3 + 2] += quantizationErrorBlue * 1 / 16;
            }
        }
    }

    // Libérer la mémoire de la CLUT
    free(clutData.clut);
    free(clutData.clutIndices);
}
