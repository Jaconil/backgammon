#ifndef _MENU
#define _MENU

// Fonction d'initialisation des noms des joueurs
void InitPlayersName(S_GameConfig* gameConfig);

// Fonctions de gestion de l'affichage du menu
void DisplayText(SDL_Surface* window, TTF_Font* font, E_GameMode gameMode, E_MenuSelected selected);
void DisplayOverlays(SDL_Surface* window, TTF_Font* font, E_MenuSelected selected, S_GameConfig gameConfig);

void DisplayMenu(SDL_Surface* window, E_GameMode gameMode, S_AIFunctions* aiFunctions);

#endif
