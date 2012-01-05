#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

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
    int finish = 0;
    SDL_Event event;
    
    while(!finish)
    {
        EventsMenu(&event, &finish);
        
        // Affichage
    }
}
