#include "starfield.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

/* STAR */

static Star Star_create(double x, double y, double z, int speed) {
    return (Star){ .x = x, .y = y, .z = z, .pz = z, .speed = speed };
}

static void Star_update(Star *star, double dt) {
    star->pz = star->z, star->z -= star->speed*dt;
}

static int map(double value, double low1, double high1, double low2, double high2) {
    return (int)(low2 + (high2 - low2) * ((value - low1) / (high1 - low1)));
}

static void Star_get(Star *star, int width, int height, int *x, int *y, int *r, int *px, int *py) {

    *x = map(star->x / star->z, 0, 1, 0, width);
    *y = map(star->y / star->z, 0, 1, 0, height);
    *r = map(star->z, 0, width, 4, 0);

    if (star->pz > width / 2.) {
        *px = -1;
        *py = -1;
    } else {
        *px = map(star->x / star->pz, 0, 1, 0, width);
        *py = map(star->y / star->pz, 0, 1, 0, height);
    }

}

/* STARIFIELD */

StarField StarField_create(int nStars, int win_w, int win_h) {

    StarField starfield = {0};

    starfield.numberOfStars = nStars;
    starfield.width = win_w;
    starfield.height = win_h;
    starfield.stars = (Star*)malloc(sizeof(Star) * nStars);

    srand(time(NULL));
    for (int i = 0; i < nStars; i++) {
        double x = ((double)rand() / RAND_MAX) * (win_w/2.) - (win_w/4.);
        double y = ((double)rand() / RAND_MAX) * win_h - (win_h/2.);
        double z = ((double)rand() / RAND_MAX) * win_w;
        int speed = rand() % 313 + 62;
        starfield.stars[i] = Star_create(x, y, z, speed);
    }

    return starfield;

}

void StarField_draw(StarField *starfield, SDL_Renderer *renderer, double dt) {

    for (int i = 0; i < starfield->numberOfStars; i++) {

        Star *star = &starfield->stars[i];

        if (star->z <= star->speed*dt) { // New star

            double x = ((double)rand() / RAND_MAX) * (starfield->width/2.) - (starfield->width/4.);
            double y = ((double)rand() / RAND_MAX) * starfield->height - (starfield->height/2.);
            double z = ((double)rand() / RAND_MAX) * starfield->width;
            int speed = rand() % 313 + 62;

            starfield->stars[i] = Star_create(x, y, z, speed);

        } else {

            Star_update(star, dt);

            int x, y, r, px, py;
            Star_get(star, starfield->width, starfield->height, &x, &y, &r, &px, &py);

            // Draw star 
            filledCircleRGBA(renderer, x, y, r, 255, 255, 255, 255);

            SDL_bool on_screen = ( //Used because thickLineRGBA creates artifacts on my hardware when one of the line points is offscreen
                px < starfield->width && x > 0 && x < starfield->width && y > 0 && y < starfield->height && py > 0 && py < starfield->height
            );

            if (px > 0 && on_screen) { // Draw star tail 
                thickLineRGBA(renderer, x, y, px, py, r, 255, 255, 255, 255);
            }

        }

    }
}

void StarField_free(StarField *starfield) {
    free(starfield->stars);
}
