#ifndef _GUI
#define _GUI

// Fonctions d'initialisation et de libération des ressources de la SDL
int InitWindow(SDL_Surface** window, SDL_Surface** icon);
void FreeWindow(SDL_Surface* icon);

#endif
