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
    void* ai[2];
    pfInitLibrary  AI_InitLibrary[2];
    pfStartMatch   AI_StartMatch[2];
    pfStartGame    AI_StartGame[2];
    pfEndGame      AI_EndGame[2];
    pfEndMatch     AI_EndMatch[2];
    pfDoubleStack  AI_DoubleStack[2];
    pfTakeDouble   AI_TakeDouble[2];
    pfMakeDecision AI_MakeDecision[2];
} S_AIFunctions;

// Fonctions d'initialisation et de terminaison de la structure des API
void InitAIStruct(S_AIFunctions* aiFunctions);
void FreeAIStruct(S_AIFunctions* aiFunctions);

int LoadAPI(char* path, S_AIFunctions* aiFunctions, int index);

E_GameMode ParseArgs(int argc, char* argv[], S_AIFunctions* aiFunctions);

#endif
