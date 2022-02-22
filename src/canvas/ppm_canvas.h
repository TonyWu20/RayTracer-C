#pragma once
#include <colors/colors.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct PPMCanvas PPMCanvas;

struct PPMCanvas
{
    int width;
    int height;
    int pixelNums;
    float *pixels;
    void (*writePixel)(PPMCanvas *self, int x, int y, Color pixelColor);
    void (*printCanvas)(PPMCanvas *self, char *fileName);
    void (*destroy)(PPMCanvas *self);
};

/* @abstract: Init a pointer of Canvas by specifiying the width and height */
static inline PPMCanvas *init_PPMCanvas(int width, int height);
/* @abstract: Write pixel to PPMCanvas buffer */
static inline void writePixel(PPMCanvas *self, int x, int y, Color pixelColor);
/* @abstract: Write pixel determined by the luma value to Canvas buffer */
static inline void printPPMCanvas(PPMCanvas *self, char *fileName);
/* @abstract: Release the memory held by the Canvas object*/
static inline void destroyPPMCanvas(PPMCanvas *self);

#pragma mark -Implementations

static inline PPMCanvas *init_PPMCanvas(int width, int height)
{
    PPMCanvas *cPtr = malloc(sizeof(PPMCanvas));
    cPtr->width = width;
    cPtr->height = height;
    cPtr->pixelNums = width * height;
    cPtr->pixels = malloc(sizeof(float) * 3 * cPtr->pixelNums);
    memset(cPtr->pixels, 0, 3 * sizeof(float) * cPtr->pixelNums);
    /* for (int i = 0; i < cPtr->pixelNums; ++i) */
    /* { */
    /*     cPtr->pixels[i] = calloc(3, sizeof(float)); */
    /* } */
    cPtr->writePixel = writePixel;
    cPtr->printCanvas = printPPMCanvas;
    cPtr->destroy = destroyPPMCanvas;
    return cPtr;
}

static inline void writePixel(PPMCanvas *self, int x, int y, Color pixelColor)
{
    if (x < 0 || x >= self->width || y < 0 || y >= self->height)
    {
        return;
    }
    int Y = self->height - 1 - y;
    int idx = Y * self->width + x;
    for (int i = 0; i < 3; ++i)
    {
        self->pixels[idx * 3 + i] = pixelColor[i];
    }
}

static inline void printPPMCanvas(PPMCanvas *self, char *fileName)
{
    FILE *f = fopen(fileName, "w");
    fprintf(f, "P3\n%d %d\n255\n", self->width, self->height);
    for (int i = 0; i < self->pixelNums; ++i)
    {
        /* float pixel = self->pixels[i * 3]; */
        /* fprintf(f, "%d %d %d ", (int)(pixel[0] * 255), (int)(pixel[1] * 255),
         */
        /* (int)(pixel[2] * 255)); */
        fprintf(f, "%d %d %d ", (int)(self->pixels[i * 3] * 255),
                (int)(self->pixels[i * 3 + 1] * 255),
                (int)(self->pixels[i * 3 + 2] * 255));
        if (i % 5 == 0 && i > 0)
            fprintf(f, "\n");
    }
    fprintf(f, "\n");
    fclose(f);
}

static inline void destroyPPMCanvas(PPMCanvas *self)
{
    /* for (int i = 0; i < self->pixelNums; ++i) */
    /* { */
    /*     free(self->pixels[i]); */
    /* } */
    free(self->pixels);
    free(self);
}
