#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "cste.h"
#include "library.h"
#include "menu.h"

/* Fonction de gestion des evenements du menu
 * @param SDL_Event* event
 *     Evenements de la fenetre
 * @param int* finish
 *     Indicateur de la fin du programme
 */
void EventsMenu(SDL_Event* event, int* finish)
{
    SDL_WaitEvent(event);
    switch(event->type)
    {
        case SDL_QUIT:
            *finish = 1;
    }
}

/* Fonction de gestion de l'affichage du menu
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param E_GameMode gameMode
 *     Mode de jeu du programme en cours
 * @param S_AIFunctions aiFunctions
 *     Structure de stockage des fonctions des bibliotheques
 */
void DisplayMenu(SDL_Surface* window, E_GameMode gameMode, S_AIFunctions* aiFunctions)
{
    SDL_Surface *menu_bg = IMG_Load(DESIGN_PATH "menu.png");
    TTF_Font *font = TTF_OpenFont(DESIGN_PATH "arial.ttf", 30);
    SDL_Color black = {0, 0, 0};
    
    SDL_Surface *txtPlayer1 = TTF_RenderText_Blended(font, "Joueur 1", black);
    
    SDL_Rect position;
    
    position.x = 0; position.y = 0;    
    SDL_BlitSurface(menu_bg, NULL, window, &position);
    
    position.x = 100; position.y = 100;
    SDL_BlitSurface(txtPlayer1, NULL, window, &position);
    
    int finish = 0;
    SDL_Event event;
    
    while(!finish)
    {
        EventsMenu(&event, &finish);
        
        // Affichage
        SDL_Flip(window);
    }
    
    SDL_FreeSurface(menu_bg);
}
