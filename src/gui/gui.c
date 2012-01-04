#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "cste.h"
#include "gui.h"

/* Fonction d'initialisation de la SDL,de la fenetre et des surfaces
 * @param S_Surfaces* surfaces
 *     Structure de stockage des surfaces SDL
 * @return int
 *     1 si aucune erreur, 0 sinon
 */
int InitWindow(S_Surfaces* surfaces)
{    
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        return 0;
    }
    
    // Chargement de l'icone
    surfaces->icon = IMG_Load(DESIGN_PATH "icon.png");
    SDL_WM_SetIcon(surfaces->icon, NULL);     
    
    // Chargement de la fenetre
    surfaces->window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Backgammon", NULL);    
    
    return 1;
}

/* Procédure pour liberer les ressources graphiques
 * @param S_Surfaces* surfaces
 *     Structure de stockage des surfaces SDL
 */
void FreeWindow(S_Surfaces* surfaces)
{
    // Désallocation des surfaces
    SDL_FreeSurface(surfaces->icon);
    
    SDL_Quit();
}

