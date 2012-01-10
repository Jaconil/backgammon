#ifndef _MENU
#define _MENU

// Fonctions de gestion des evenements, et de l'affichage du menu
void TextInput(char* name, SDL_keysym key);
E_MenuSelected EventsMenu(SDL_Event* event, S_GameConfig* gameConfig, E_MenuSelected* selected);

void DisplayText(SDL_Surface* window, TTF_Font* font, E_GameMode gameMode, E_MenuSelected selected);
void DisplayOverlays(SDL_Surface* window, TTF_Font* font, E_MenuSelected selected, S_GameConfig gameConfig);
void InitPlayersName(S_GameConfig* gameConfig, E_GameMode gameMode, S_AIFunctions* aiFunctions);
void DisplayMenu(SDL_Surface* window, E_GameMode gameMode, S_AIFunctions* aiFunctions);

#endif
