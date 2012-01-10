#ifndef _GUI
#define _GUI

// Fonctions d'initialisation et de libération des ressources de la SDL
int InitWindow(SDL_Surface** window, SDL_Surface** icon);
void FreeWindow(SDL_Surface* icon);

int ClickRect(SDL_Event* event, int x, int y, int w, int h);
int ClickZone(SDL_Event* event);
int IsValidSrc(int zone, S_GameState* gameState);

#endif
