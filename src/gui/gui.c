#include <stdio.h>
#include <SDL/SDL.h>

#include "cste.h"
#include "gui.h"

/* Fonction d'initialisation de la SDL et de la fenetre
 * @return int
 *     1 si aucune erreur, 0 sinon
 */
int InitWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL\n");
        return 0;
    }
    
    SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    
    
    
    return 1;
}

/* Procédure pour libérer les ressources graphiques
 */
void FreeWindow()
{
    SDL_Quit();
}

