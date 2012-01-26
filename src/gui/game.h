#ifndef _GAME
#define _GAME

void RollDice(S_GameState* gameState);

int IsValidSrc(int zone, S_GameState* gameState);
int IsValidDst(int zone, S_GameState* gameState);
int IsPossibleMove(S_GameState* gameState);
int CanBearOff(S_GameState* gameState);
int IsFinish(S_GameState* gameState);

void DoMove(int zone, S_GameState* gameState);

#endif