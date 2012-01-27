#ifndef _GUI
#define _GUI

// Fonctions d'initialisation et de libération des ressources de la SDL
int InitWindow(SDL_Surface** window, SDL_Surface** icon);
void FreeWindow(SDL_Surface* icon);

// Fonction de saisie de nom
void TextInput(char* name, SDL_keysym key);

// Fonctions de positionnement de clic
int ClickRect(SDL_Event* event, int x, int y, int w, int h);
int ClickZone(SDL_Event* event);

// Fonctions d'affichage de boutons et popup
void DisplayButton(SDL_Surface* window, int x, int y, char* label, int select);
void DisplayPopup(SDL_Surface* window, int nb, ...);

#endif
