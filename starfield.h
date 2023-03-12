#ifndef STARFIELD_H
#define STARFIELD_H

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

/* Star */

typedef struct Star {
    double x;
    double y;
    double z;
    double pz;
    int speed;
} Star;

Star *Star_create(double x, double y, double z, int speed);
void Star_update(Star *star, double dt);
void Star_get(Star *star, int width, int height, int *x, int *y, int *r, int *px, int *py);

/* Starfield */

typedef struct StarField {
    int numberOfStars;
    int width;
    int height;
    Star **stars;
} StarField;

StarField *StarField_create(int nStars, int width, int height);
void StarField_draw(StarField *starfield, SDL_Renderer *renderer, double dt);
void StarField_free(StarField *starfield);

#endif /* STARFIELD_H */
