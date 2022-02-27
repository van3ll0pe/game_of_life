#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include "screen.h"
#include "cellar.h"

#define SIZE_CELLAR 16

typedef struct Map Map;

typedef enum STATE_GAME {
    PAUSE,
    RUNNING
} STATE_GAME;

typedef enum STATE_CLICK {
    NOT_CLICKED,
    CLICKED
} STATE_CLICK;

struct Map {
    SDL_Texture     *m_texture_map;     //the texture to draw all cellars
    Cellar          **m_map_cellar;     //the map 2D of cellar
    Cellar          **m_next_map_cellar;    //the map 2D of cellar after apply rules
    int             m_width_map;        //the width limit of the 2D map of cellar    
    int             m_height_map;       //the height limit of the 2D map of cellar
    char            m_status_game;           //the status of the GAME, is launching or not
    char            m_status_click;         //the status of the interruption click mouse (only when the game is on pause)

    Vector2i        m_position_mouse;   //the position of mouse on the screen

    void            (*change_state_game)(Map *map); //pointer of function map_change_state_game
    void            (*change_value_cellar)(Map *map); //pointer of function map_change_value_cellar
    void            (*color_cellar)(Map *map, Screen *screen); //pointer of function map_color_cellar
    void            (*display_texture)(Map *map, Screen *screen); //pointer of function map_display_texture
    void            (*rule_gamelife)(Map *map); //pointer of function map_rule_gamelife
    void            (*draw_lines)(Map *map, Screen *screen); //pointer of function map_draw_lines
    void            (*destroy_map)(Map *map); //pointer of function map_destroy_map


};

Map init_map(SDL_Renderer *render, int w, int h); //the initialisation of the structure, X is the width of the screen and y is the height of the screen
void map_change_state_game(Map *map); //the function to change the state of the game to launch or pause the game
void map_change_value_cellar(Map *map); //the function to change to state of the cellar when the game is on PAUSE
void map_color_cellar(Map *map, Screen *screen); //function to color on the texture each cellar in while the status of it
void map_display_texture(Map *map, Screen *screen); //function to display the texture of the map on the renderer of the screen
void map_rule_gamelife(Map *map); //function to apply the rules of the game of life
void map_draw_lines(Map *map, Screen *screen); //function to draw lines between square cellar

void map_destroy_map(Map *map); //function to free each pointers

#endif