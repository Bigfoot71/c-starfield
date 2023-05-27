#ifndef STARFIELD_H
#define STARFIELD_H

#include <SDL2/SDL.h>

/* Starfield */

typedef struct Star {
  double x, y, z, pz;
  int speed;
} Star;

typedef struct StarField {
    int numberOfStars;
    int width, height;
    Star *stars;
} StarField;

StarField StarField_create(int nStars, int width, int height);
void StarField_draw(StarField *starfield, SDL_Renderer *renderer, double dt);
void StarField_free(StarField *starfield);

#endif /* STARFIELD_H */
