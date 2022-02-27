#include <stdio.h>
#include <stdlib.h>

#include "../include/screen.h"
#include "../include/map.h"


int init_SDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
    {
        printf("probleme d'initialisation de la SDL\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


void getEvent(SDL_Event *event, Screen *screen, Map *map) {

    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) { //si on quit la fenetre
            screen->m_isRunning = 0;
        }

        if (event->type == SDL_MOUSEBUTTONDOWN) { 
            if (event->button.button == SDL_BUTTON_LEFT) { //si la personne fait un click gauche
                if (map->m_status_game == PAUSE) {
                    map->m_status_click = (char)CLICKED;
                    SDL_GetMouseState(&map->m_position_mouse.x, &map->m_position_mouse.y);
                    printf("mouse at : %d - %d\n", map->m_position_mouse.x, map->m_position_mouse.y);
                }
            }
        }
    }
}


int main()
{
    if (init_SDL() != 0)
    {
        return EXIT_FAILURE;
    }


    Screen application = screen_init(928, 608, "GameLife");
    if (application.init_sdl(&application) != 0)
    {
        return EXIT_SUCCESS;
    }

    Map map = init_map(application.m_render, application.m_widthWindow, application.m_heightWindow);
    SDL_Event application_event;

    //main loop
    while (application.m_isRunning)
    {
        getEvent(&application_event, &application, &map);

        application.clear(&application);
        
        map.change_state_game(&map);
        map.change_value_cellar(&map);
        map.color_cellar(&map, &application);
        map.rule_gamelife(&map);
        map.draw_lines(&map, &application);

        map.display_texture(&map, &application);
        SDL_RenderPresent(application.m_render);
    }

    map.destroy_map(&map);
    application.destroy(&application);
    SDL_Quit();

    return EXIT_SUCCESS;
}