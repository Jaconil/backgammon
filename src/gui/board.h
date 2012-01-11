#ifndef _BOARD
#define _BOARD

void InitGameState(S_GameState* gameState, S_GameConfig gameConfig);
void DisplayCheckers(SDL_Surface* window, S_GameState gameState);
void DisplayBoardOverlays(SDL_Surface* window, S_GameState gameState);
void DisplayDice(SDL_Surface* window, S_GameState gameState);
void DisplayNumbers(SDL_Surface* window, S_GameState gameState);

int DisplayBoard(SDL_Surface* window, E_GameMode gameMode, S_AIFunctions* aiFunctions, S_GameConfig gameConfig);

E_BoardSelected EventsBoard(SDL_Event* event, S_GameState* gameState);

void RollDice(S_GameState* gameState);

#endif
