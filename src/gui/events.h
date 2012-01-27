#ifndef _EVENTS
#define EVENTS

// Fonction de gestion d'evenemement du menu
E_MenuSelected EventsMenu(SDL_Event* event, S_GameConfig* gameConfig, E_MenuSelected* selected);

// Fonctions de gestion d'evenemements du plateau
E_BoardSelected EventsBoard(SDL_Event* event, S_GameState* gameState);
void AI_EventsBoard(S_GameState* gameState);

#endif