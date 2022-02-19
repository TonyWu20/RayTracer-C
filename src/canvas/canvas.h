#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _Canvas Canvas;
struct _Canvas
{
    int width;
    int height;
    int pixelNums;
    char *pixels;
    void (*writePixel)(Canvas *self, int x, int y, char symbol);
    void (*show)(Canvas *self);
    void (*destroy)(Canvas *self);
};
/* @abstract: Init a pointer of Canvas by specifiying the width and height */
static inline Canvas *initCanvas(int width, int height);
/* @abstract: Write pixel to Canvas buffer */
static inline void writeToCanvas(Canvas *self, int x, int y, char symbol);
/* @abstract: Print the content of Canvas to terminal */
static inline void printCanvas(Canvas *self);
/* @abstract: Release the memory held by the Canvas object*/
static inline void destroyCanvas(Canvas *self);

/* @abstract: Init a pointer of Canvas by specifiying the width and height */
static inline Canvas *initCanvas(int width, int height)
{
    Canvas *cPtr = malloc(sizeof(Canvas));
    cPtr->width = width;
    cPtr->height = height;
    cPtr->pixelNums = width * height;
    cPtr->pixels = malloc(width * height * sizeof(char));
    memset(cPtr->pixels, ' ', width * height);
    cPtr->writePixel = writeToCanvas;
    cPtr->show = printCanvas;
    cPtr->destroy = destroyCanvas;
    return cPtr;
}
static inline void writeToCanvas(Canvas *self, int x, int y, char symbol)
{
    if (x < 0 || x >= self->width || y < 0 || y >= self->height)
    {
        return;
    }
    int Y = self->height - 1 - y;
    self->pixels[Y * self->width + x] = symbol;
}
static inline void destroyCanvas(Canvas *self)
{
    free(self->pixels);
    free(self);
}
static inline void printCanvas(Canvas *self)
{
    printf("\x1b[2J\x1b[H"); // Clear screen
    for (int i = 0; i < self->pixelNums; ++i)
    {
        putchar((i % self->width) ? self->pixels[i] : 10);
    }
}
