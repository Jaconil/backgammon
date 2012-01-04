#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>

#include "cste.h"
#include "library.h"
#include "gui.h"
 
int main(int argc, char *argv[])
{
    S_AIFunctions aiFunctions;
    S_Surfaces surfaces;
    
    InitAIStruct(&aiFunctions);

    // Analyse des arguments et chargement des API
    E_GameMode gameMode = ParseArgs(argc, argv, &aiFunctions);
    
    if (gameMode == ERROR)
        return EXIT_FAILURE;
    
    if (!InitWindow(&surfaces))
        return EXIT_FAILURE;
        
    //DisplayMenu();
    
    // Code temporaire
    int continuer = 1;
    SDL_Event event;
    
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
    // fin code temporaire
    
    // Libération des ressources
    FreeWindow(&surfaces);    
    FreeAIStruct(&aiFunctions);

    return EXIT_SUCCESS;
}
