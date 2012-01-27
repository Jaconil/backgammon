#ifndef _BOARD
#define _BOARD

// Fonction d'initialisation de l'etat du jeu
void InitGameState(S_GameState* gameState, S_GameConfig gameConfig);

// Fonctions d'affichage
void DisplayCheckers(SDL_Surface* window, S_GameState gameState);
void DisplayBoardOverlays(SDL_Surface* window, S_GameState gameState);
void DisplayDice(SDL_Surface* window, S_GameState gameState);
void DisplayNumbers(SDL_Surface* window, S_GameState gameState);
void DisplayHelp(SDL_Surface* window, S_GameState gameState);
void DisplayScore(SDL_Surface* window, S_GameState gameState);

int DisplayBoard(SDL_Surface* window, S_GameConfig gameConfig);

#endif
