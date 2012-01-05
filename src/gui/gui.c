#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "cste.h"
#include "gui.h"

/* Fonction d'initialisation de la SDL,de la fenetre et des surfaces
 * @param SDL_Surface* window
 *     Surface de la fenetre du programme
 * @param SDL_Surface* icon
 *     Surface de l'icone du programme
 * @return int
 *     1 si aucune erreur, 0 sinon
 */
int InitWindow(SDL_Surface* window, SDL_Surface* icon)
{    
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        return 0;
    }
    
    // Chargement de l'icone
    icon = IMG_Load(DESIGN_PATH "icon.png");
    SDL_WM_SetIcon(icon, NULL);     
    
    // Chargement de la fenetre
    window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    
    if (window == NULL)
    {
        fprintf(stderr, "Erreur d'ouverture du mode vidéo : %s\n", SDL_GetError());
        return 0;
    }
    
    SDL_WM_SetCaption("Backgammon", NULL);    
    
    return 1;
}

/* Procédure pour liberer les ressources graphiques
 * @param SDL_Surface* icon
 *     Surface de l'icone du programme
 */
void FreeWindow(SDL_Surface* icon)
{
    // Desallocation de l'icone
    SDL_FreeSurface(icon);
    
    SDL_Quit();
}

