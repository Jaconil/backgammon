#ifndef _MENU
#define _MENU

// Partie du formulaire qui est sélectionée
typedef enum
{
    PLAYER1,
    PLAYER2,
    OTHER
} E_MenuSelected;

typedef enum
{
    BLACK,
    WHITE
} E_PlayerColor;

typedef struct
{
    char namePlayer1[14];
    char namePlayer2[14];
    E_PlayerColor player1Color;
    int option;
    int points;
} S_GameConfig;

// Fonctions de gestion des evenements, et de l'affichage du menu
int ClickRect(SDL_Event* event, int x, int y, int w, int h);
void TextInput(char* name, SDL_keysym key);
void EventsMenu(SDL_Event* event, int* finish, S_GameConfig* gameConfig, E_MenuSelected* selected);

void DisplayText(SDL_Surface* window, TTF_Font* font, E_GameMode gameMode);
void DisplayOverlays(SDL_Surface* window, TTF_Font* font, E_MenuSelected selected, S_GameConfig gameConfig);
void InitPlayersName(S_GameConfig* gameConfig, E_GameMode gameMode, S_AIFunctions* aiFunctions);
void DisplayMenu(SDL_Surface* window, E_GameMode gameMode, S_AIFunctions* aiFunctions);

#endif
