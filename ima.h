#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include <string.h>





typedef struct {
    GLubyte red;
    GLubyte green;
    GLubyte blue;
} Pixel;

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    GLubyte *data;
};

typedef struct Image Image;

typedef struct {
    Pixel *clut;
    int *clutIndices;
    int clutSize;
} ClutValue;

typedef struct Site {
    GLubyte r;
    GLubyte g;
    GLubyte b;
    unsigned long x;
    unsigned long y;
} Site;

typedef struct Vclut {
    Site *sites;
    unsigned long sitesNmb;
    unsigned long sizeX;
    unsigned long sizeY;
} Vclut;




int ImageLoad_PPM(char *filename, Image *image);
void imagesave_PPM(char *filename, Image *image);
void upsidedown(Image *);
void dithering(Image *im);



