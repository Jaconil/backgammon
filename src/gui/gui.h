#ifndef _GUI
#define _GUI

typedef struct
{
    SDL_Surface *window;
    SDL_Surface *icon;
    
} S_Surfaces;

// Fonctions d'initialisation et de libération des ressources de la SDL
int InitWindow(S_Surfaces* surfaces);
void FreeWindow(S_Surfaces* surfaces);

#endif
