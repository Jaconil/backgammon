#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

// Compatibilité Windows
#ifdef _WIN32
#undef main
#endif

#include "cste.h"
#include "library.h"
#include "gui.h"
#include "menu.h"

int main(int argc, char *argv[])
{
    // Analyse des arguments et chargement des API
    S_AIFunctions aiFunctions[2];    
    InitAIStruct(aiFunctions);

    E_GameMode gameMode = ParseArgs(argc, argv, aiFunctions);
    
    if (gameMode == ERROR)
        return EXIT_FAILURE;
        
    // Chargement de la fenetre
    SDL_Surface *window = NULL;
    SDL_Surface *icon = NULL;
    
    if (!InitWindow(&window, &icon))
        return EXIT_FAILURE;
    
    // Affichage du menu
    DisplayMenu(window, gameMode, aiFunctions);
    
    // Liberation des ressources
    FreeWindow(icon);    
    FreeAIStruct(aiFunctions);

    return EXIT_SUCCESS;
}
