#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "library.h"
 
int main(int argc, char *argv[])
{
    S_AIFunctions aiFunctions;
    
    InitAIStruct(&aiFunctions);

    // Analyse des arguments et chargement des API
    E_GameMode gameMode = ParseArgs(argc, argv, &aiFunctions);
    
    if (gameMode == ERROR)
        return EXIT_FAILURE;
    
    // Affichage du menu
    //InitWindow();
    //DisplayMenu();
    
    FreeAIStruct(&aiFunctions);

    return EXIT_SUCCESS;
}
