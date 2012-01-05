#ifndef _MENU
#define _MENU

// Fonctions de gestion des evenements, et de l'affichage du menu
void EventsMenu(SDL_Event* event, int* finish);
void DisplayMenu(SDL_Surface* window, E_GameMode gameMode, S_AIFunctions* aiFunctions);

#endif
