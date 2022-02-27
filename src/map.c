#include "../include/map.h"

/**
 * @brief Function to init a struct Map ||Map
 * 
 * @param render 
 * @param x (the size of the screen in width)
 * @param y (the size of the screen in height)
 * @return Map 
 */
Map init_map(SDL_Renderer *render, int w, int h) {
    Map map;

    map.m_status_game = (char)PAUSE; //le status du jeu
    map.m_status_click = (char)NOT_CLICKED; //the status des cliques souris
    map.m_width_map = w / SIZE_CELLAR; //le nombre de cellule en longueur (w est la largeur de l'écran)
    map.m_height_map = h / SIZE_CELLAR; //le nombre de cellule en hauteur (h est la hauteur de l'écran)

    map.m_position_mouse.x = -1;
    map.m_position_mouse.y = -1;
    
    //creation de la texture pour afficher nos cellules
    map.m_texture_map = NULL;
    map.m_texture_map = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    if (map.m_texture_map == NULL) {
        fprintf(stderr, "Erreur création de la texture\n");
        exit(1);
    }

    //---------------creation du double tableau de cellule-----------------
    map.m_map_cellar = NULL;
    map.m_map_cellar = malloc(sizeof(Cellar *) * map.m_height_map);
    if (map.m_map_cellar == NULL) {
        fprintf(stderr, "Problème creation de la map\n");
        exit(1);
    }

    //creation de chaque ligne du tableau avec une initialisation de chaque element
    for (int y = 0; y < map.m_height_map; y++) {
        map.m_map_cellar[y] = malloc(sizeof(Cellar) * map.m_width_map); //malloc de chaque ligne
        if (map.m_map_cellar[y] == NULL) {
            fprintf(stderr, "Problème creation de la map\n");
            exit(1);
        }

        for (int x = 0; x < map.m_width_map; x++) { //initialisation de chaque bloc de la ligne
            map.m_map_cellar[y][x] = init_cellar((((x+1)*SIZE_CELLAR) - SIZE_CELLAR), (((y+1)*SIZE_CELLAR) - SIZE_CELLAR));
        }
    }
    //------------------------------------------------------------
    //-------- initialisation du next_map_cellar------------------
    map.m_next_map_cellar = NULL;
    map.m_next_map_cellar = malloc(sizeof(Cellar *) * map.m_height_map);
    if (map.m_next_map_cellar == NULL) {
        fprintf(stderr, "Problème creation de la map\n");
        exit(1);
    }

    //creation de chaque ligne du tableau avec une initialisation de chaque element
    for (int y = 0; y < map.m_height_map; y++) {
        map.m_next_map_cellar[y] = malloc(sizeof(Cellar) * map.m_width_map); //malloc de chaque ligne
        if (map.m_next_map_cellar[y] == NULL) {
            fprintf(stderr, "Problème creation de la map\n");
            exit(1);
        }

        for (int x = 0; x < map.m_width_map; x++) { //initialisation de chaque bloc de la ligne
            map.m_next_map_cellar[y][x] = init_cellar((((x+1)*SIZE_CELLAR) - SIZE_CELLAR), (((y+1)*SIZE_CELLAR) - SIZE_CELLAR));
        }
    }
    //--------------------------------------------------------------------
    //initialisation des pointeurs de fonction
    map.change_state_game = &map_change_state_game;
    map.change_value_cellar = &map_change_value_cellar;
    map.color_cellar = &map_color_cellar;
    map.display_texture = &map_display_texture;
    map.rule_gamelife = &map_rule_gamelife;
    map.draw_lines = &map_draw_lines;
    map.destroy_map = &map_destroy_map;

    return map;
}


/**
 * @brief function to change the state of the GAME.
 * If the game is on PAUSE and the player press SPACE key the game will start but if the game is launching and the player press SPACE key the game will go in sleep mode
 * 
 */
void map_change_state_game(Map *map) {
    const unsigned char *keyboard_state = SDL_GetKeyboardState(NULL);
    static uint32_t time_save = 0;
    uint32_t time_actual = 0;

    time_actual = SDL_GetTicks();

    if (time_save + 150 <= time_actual)
    {
        time_save = time_actual;
        if (map->m_status_game == PAUSE && keyboard_state[SDL_SCANCODE_SPACE]) { //if the game is on pause and the player press space
            map->m_status_game = RUNNING;
            printf("game is running now\n");
        } else if (map->m_status_game == RUNNING && keyboard_state[SDL_SCANCODE_SPACE]) { //if the game is running and the player press space
            map->m_status_game = PAUSE;
            printf("game is on PAUSE now\n");
        }
    }
}



/**
 * @brief Function to change the status of each cellar while the game is not running
 * 
 * @param map 
 */
void map_change_value_cellar(Map *map) {

    if (map->m_status_game == PAUSE && map->m_status_click == CLICKED) { //si le jeu est en pause et que la personne a cliqué (le click permet de ne pas parcourir la boucle si on clique pas même si on est en pause)
        map->m_status_click = NOT_CLICKED;
        //on parcourt le tableau de tableau et on change le status de la cellule qui est pointé par la souris
        for (int y = 0; y < map->m_height_map; y++) {
            for (int x = 0; x < map->m_width_map; x++) {
                if ((map->m_position_mouse.x > map->m_map_cellar[y][x].position.x && map->m_position_mouse.x < map->m_map_cellar[y][x].position.x + SIZE_CELLAR) && (map->m_position_mouse.y > map->m_map_cellar[y][x].position.y && map->m_position_mouse.y < map->m_map_cellar[y][x].position.y + SIZE_CELLAR)) {

                    if (map->m_map_cellar[y][x].status == DEAD) {
                        map->m_map_cellar[y][x].status = LIVE;
                        printf("cellar at position %d - %d is now : LIVE\n", map->m_map_cellar[y][x].position.x, map->m_map_cellar[y][x].position.y);
                    } else {
                        map->m_map_cellar[y][x].status = DEAD;
                        printf("cellar at position %d - %d is now : DEAD\n", map->m_map_cellar[y][x].position.x, map->m_map_cellar[y][x].position.y);
                    }
                }
            }
        }
    }
}

/**
 * @brief color each square cellar on the texture instead the status of cellar
 * 
 * @param map 
 * @param screen 
 */
void map_color_cellar(Map *map, Screen *screen) {
    SDL_SetRenderTarget(screen->m_render, map->m_texture_map); //notre texture est la cible pour dessiner

    for (int y = 0; y < map->m_height_map; y++) {
        for (int x = 0; x < map->m_width_map; x++) {
            SDL_Rect rectangle_cellule = {.x = map->m_map_cellar[y][x].position.x, .y = map->m_map_cellar[y][x].position.y, .h = SIZE_CELLAR, .w = SIZE_CELLAR};

            if (map->m_map_cellar[y][x].status == LIVE) { //si la cellule est en vie, on dessine un carré blanc
                SDL_SetRenderDrawColor(screen->m_render, 0, 0, 0, 255);    
            } 
            else { //si la cellule est morte on dessine un carré noir
                SDL_SetRenderDrawColor(screen->m_render, 255, 255, 255, 255);
            }

            SDL_RenderFillRect(screen->m_render, &rectangle_cellule); //on dessine sur la texture
        }
    }

    SDL_SetRenderTarget(screen->m_render, NULL); //le renderer redevient la cible
}

/**
 * @brief function to apply the texture of the map, on the screen
 * 
 * @param map 
 * @param screen 
 */
void map_display_texture(Map *map, Screen *screen) {
    SDL_RenderCopy(screen->m_render, map->m_texture_map, NULL, NULL);
}

//function to apply

void map_rule_gamelife(Map *map) {

    static uint32_t time_save = 0; //le temps sauvegardé
    uint32_t actual_time = 0; //le temps actuel
    Cellar **tmp = NULL; //un double pointeur temporaire pour echanger les adresses mémoires des deux tableaux

    if (map->m_status_game == RUNNING) {
        actual_time = SDL_GetTicks();

        if (time_save + 300 <= actual_time) { //on gère la vitesse d'animation des cellules toutes les 600 millisecondes
            time_save = actual_time;

            for (int y = 0; y < map->m_height_map; y++) { //on parcourt le tableau pour checker les états des cellules
                
                for (int x = 0; x < map->m_width_map; x++) {
                    int nbr_life_beside = 0;
                    //check des status des cellules voisines
                    if (x == 0 && y == 0) //le coin en haut à gauche
                    { 
                        if (map->m_map_cellar[y+1][x].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x+1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y][x+1].status == LIVE)
                            nbr_life_beside++;
                    }
                    else if (x == map->m_width_map-1 && y == 0) //le coin en haut à droite
                    {
                        if (map->m_map_cellar[y][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x].status == LIVE)
                            nbr_life_beside++;
                    }
                    else if (x == 0 && y == map->m_height_map - 1) //le coin en bas à gauche
                    {
                        if (map->m_map_cellar[y-1][x].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y-1][x+1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y][x+1].status == LIVE)
                            nbr_life_beside++;

                    }
                    else if (x == map->m_width_map - 1 && y == map->m_height_map-1) //le coin en bas à droite
                    {
                        if (map->m_map_cellar[y-1][x].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y-1][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y][x-1].status == LIVE)
                            nbr_life_beside++;
                    }

                    else if (y == 0 && (x > 0 && x < map->m_width_map-1)) //la première ligne horizontale sans les angles
                    {
                        if (map->m_map_cellar[y][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x+1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y][x+1].status == LIVE)
                            nbr_life_beside++;

                    }
                    else if (y == map->m_height_map-1 && (x > 0 && x < map->m_width_map-1)) //la dernière ligne horizontale sans les angles
                    {
                        if (map->m_map_cellar[y][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y-1][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y-1][x].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y-1][x+1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y][x+1].status == LIVE)
                            nbr_life_beside++;

                    }
                    else if (x == 0 && (y > 0 && y < map->m_height_map-1)) //la première ligne verticale sans les angles
                    {
                        if (map->m_map_cellar[y-1][x].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y-1][x+1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y][x+1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x+1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x].status == LIVE)
                            nbr_life_beside++;


                    }
                    else if (x == map->m_width_map-1 && (y > 0 && y < map->m_height_map-1)) //la dernière ligne verticale sans les angles
                    {
                        if (map->m_map_cellar[y-1][x].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y-1][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x].status == LIVE)
                            nbr_life_beside++;

                    }
                    else
                    {
                        if (map->m_map_cellar[y-1][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y-1][x].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y-1][x+1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y][x+1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x-1].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x].status == LIVE)
                            nbr_life_beside++;
                        if (map->m_map_cellar[y+1][x+1].status == LIVE)
                            nbr_life_beside++;
                    }
                    //fin des checks des cellules voisines


                    if (map->m_map_cellar[y][x].status == DEAD && nbr_life_beside == 3) //SI la cellule est morte et qu'elle possède trois voisines vivantes
                    {
                        map->m_next_map_cellar[y][x].status = LIVE;
                    }
                    else if (map->m_map_cellar[y][x].status == LIVE && (nbr_life_beside == 2 || nbr_life_beside == 3))
                    {
                        map->m_next_map_cellar[y][x].status = LIVE;
                    }
                    else
                    {
                        map->m_next_map_cellar[y][x].status = DEAD;
                    }




                }

            }

            tmp = map->m_map_cellar;
            map->m_map_cellar = map->m_next_map_cellar;
            map->m_next_map_cellar = tmp;

        }
    }
}

/**
 * @brief draw lines horizontal and vertical to separate each square
 * 
 * @param map 
 * @param screen 
 */
void map_draw_lines(Map *map, Screen *screen) {
    SDL_SetRenderTarget(screen->m_render, map->m_texture_map); //la texture devient la cible pour dessiner
    SDL_SetRenderDrawColor(screen->m_render, 128, 128, 128, 150);
    //les lignes verticales
    for (int i = 0; i < screen->m_widthWindow; i+= SIZE_CELLAR) {
        SDL_RenderDrawLine(screen->m_render, i, 0, i, screen->m_heightWindow);
    }

    //les lignes horizontales
    for (int i = 0; i < screen->m_heightWindow; i+= SIZE_CELLAR) {
        SDL_RenderDrawLine(screen->m_render, 0, i, screen->m_widthWindow, i);

    }

    SDL_SetRenderTarget(screen->m_render, NULL);
}

/**
 * @brief function to deallocate memory
 * 
 * @param map 
 */
void map_destroy_map(Map *map) {
    for (int i = 0; i < map->m_height_map; i++) {
        free(map->m_map_cellar[i]);
        free(map->m_next_map_cellar[i]);
    }

    free(map->m_map_cellar);
    free(map->m_next_map_cellar);
}