#ifndef _LIBRARY
#define _LIBRARY

#include "backgammon.h"

// Style de jeu
typedef enum
{
    HUMAN_HUMAN,
    HUMAN_AI,
    AI_AI,
    ERROR
} E_GameMode;

// Stockage des API et des pointeurs de fonctions pour les 2 IA
typedef struct
{
    void* ai;
    pfInitLibrary  AI_InitLibrary;
    pfStartMatch   AI_StartMatch;
    pfStartGame    AI_StartGame;
    pfEndGame      AI_EndGame;
    pfEndMatch     AI_EndMatch;
    pfDoubleStack  AI_DoubleStack;
    pfTakeDouble   AI_TakeDouble;
    pfMakeDecision AI_MakeDecision;
} S_AIFunctions;

// Fonctions d'initialisation et de terminaison de la structure des API
void InitAIStruct(S_AIFunctions* aiFunctions);
void FreeAIStruct(S_AIFunctions* aiFunctions);

int LoadAPI(char* path, S_AIFunctions* aiFunctions, int index);

E_GameMode ParseArgs(int argc, char* argv[], S_AIFunctions* aiFunctions);

#endif
