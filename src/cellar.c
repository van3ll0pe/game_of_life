#include "../include/cellar.h"


/**
 * @brief Function to init the structure Cellar || Cellar.
 * 
 * @param x (la position dans la map)
 * @param y (la position dans la map)
 * @return Cellar 
 */
Cellar init_cellar(int x, int y) {
    Cellar cellule = {
                        .position.x = x,
                        .position.y = y,
                        .status = (char)DEAD
                    };

    return cellule;
}