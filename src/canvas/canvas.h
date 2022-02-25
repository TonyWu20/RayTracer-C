#pragma once
#include <colors/colors.h>
#include <simd/simd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _Canvas Canvas;
struct _Canvas
{
    int width;
    int height;
    int pixelNums;
    char **pixels;
    void (*writeLumaPixel)(Canvas *self, int x, int y, Color pixel);
    void (*show)(Canvas *self);
    /* void (*clear)(Canvas *self); */
    void (*destroy)(Canvas *self);
};
/* @abstract: Init a pointer of Canvas by specifiying the width and height */
static inline Canvas *init_Canvas(int width, int height);
/* @abstract: Write pixel to Canvas buffer */
static inline void writeToCanvas(Canvas *self, int x, int y, char symbol);
/* @abstract: Write pixel determined by the luma value to Canvas buffer */
static inline void writeToCanvas_luma(Canvas *self, int x, int y, Color pixel);
/* @abstract: Print the content of Canvas to terminal */
static inline void printCanvas(Canvas *self);
/* @abstract: Clear the content of Canvas to terminal */
static inline void clearCanvas(Canvas *self);
/* @abstract: Release the memory held by the Canvas object*/
static inline void destroyCanvas(Canvas *self);

/* @abstract: Init a pointer of Canvas by specifiying the width and height */
static inline Canvas *init_Canvas(int width, int height)
{
    Canvas *self = malloc(sizeof(Canvas));
    self->width = width;
    self->height = height;
    self->pixelNums = width * height;
    self->pixels = malloc(width * height * sizeof(char *));
    for (int i = 0; i < self->pixelNums; ++i)
    {
        self->pixels[i] = malloc(sizeof(char) * 48);
        snprintf(self->pixels[i], 2, " ");
    }
    self->writeLumaPixel = writeToCanvas_luma;
    self->show = printCanvas;
    /* self->clear = clearCanvas; */
    self->destroy = destroyCanvas;
    return self;
}
static inline void writeToCanvas_luma(Canvas *self, int x, int y, Color pixel)
{
    if (x < 0 || x >= self->width || y < 0 || y >= self->height)
    {
        return;
    }
    pixel = simd_clamp(pixel, 0, 1);
    char symbol = lumaToChar(pixel);
    pixel = 255 * pixel;
    int Y = self->height - 1 - y;
    char format[] = "\x1b[38;2;%d;%d;%dm%c\x1b[0m";
    int sLen = 1 + snprintf(NULL, 0, format, (int)pixel.x, (int)pixel.y,
                            (int)pixel.z, symbol);
    int idx = Y * self->width + x;
    snprintf(self->pixels[idx], sLen, format, (int)pixel.x, (int)pixel.y,
             (int)pixel.z, symbol);
    /* self->pixels[Y * self->width + x] = ".,-~:;=!*#$@"[luma > 0 ? luma : 0];
     */
}
static inline void clearCanvas(Canvas *self)
{
    memset(self->pixels, ' ', self->pixelNums);
    return;
}
static inline void destroyCanvas(Canvas *self)
{
    for (int i = 0; i < self->pixelNums; ++i)
    {
        free(self->pixels[i]);
    }
    free(self->pixels);
    free(self);
}
static inline void printCanvas(Canvas *self)
{
    printf("\x1b[2J");
    printf("\x1b[H"); // Clear screen
    for (int i = 0; i < self->pixelNums; ++i)
    {
        printf("%s", (i % self->width) ? self->pixels[i] : "\n");
    }
}
