#ifndef CELLAR_H
#define CELLAR_H

typedef enum STATE_LIFE_CELLAR {
    DEAD, //constante permettant de savoir si une cellule est morte (0)
    LIVE, //constante permettant de savoir si une cellule est vivante (1)
}STATE_LIFE_CELLAR;

typedef struct Vector2i {
    int x;
    int y;
} Vector2i;

typedef struct Cellar {
    Vector2i    position; //the position of the cellar
    char        status;        //if the cellar is dead or in live (for the color to display)
} Cellar;


Cellar init_cellar(int x, int y); //the initialisation of the struct Cellar, x is the Position on the 2D map and y is the position on the 2D map




#endif