#include <stdlib.h>
#include <stdio.h>

#include "cste.h"
#include "library.h"
#include "gui.h"
 
int main(int argc, char *argv[])
{
    S_AIFunctions aiFunctions;
    
    InitAIStruct(&aiFunctions);

    // Analyse des arguments et chargement des API
    E_GameMode gameMode = ParseArgs(argc, argv, &aiFunctions);
    
    if (gameMode == ERROR)
        return EXIT_FAILURE;
    
    if (!InitWindow())
        return EXIT_FAILURE;
        
    //DisplayMenu();
    while(1);
    
    // Libération des ressources
    FreeWindow();    
    FreeAIStruct(&aiFunctions);

    return EXIT_SUCCESS;
}
