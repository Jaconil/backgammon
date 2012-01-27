#ifndef _GAME
#define _GAME

// Fonction de lancer de des
void RollDice(S_GameState* gameState);

// Fonctions de verification des mouvements
int IsValidSrc(int zone, S_GameState* gameState);
int IsValidDst(int zone, S_GameState* gameState);
int IsPossibleMove(S_GameState* gameState);
int CanBearOff(S_GameState* gameState);

// Fonctions de gestion de la sortie et des points gagnes
int IsFinish(S_GameState* gameState);
int GetPoints(S_GameState* gameState);

// Fonction qui effectue le mouvement
void DoMove(int zone, S_GameState* gameState);

// Fonction qui verifie si un joueur est humain
int IsHuman(S_GameState* gameState, int current);

// Fonctions de l'arbitrage des IA
void CreateAIGameState(SGameState* AI_gameState, S_GameState* gameState);
int ConvertAIZone(S_GameState* gameState, EPosition zone);
int IsValidAIMoves(S_GameState gameState, SMove moves[4]);

#endif