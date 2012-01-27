#ifndef _LIBRARY
#define _LIBRARY

// Fonctions d'initialisation et de terminaison de la structure des API
void InitAIStruct(S_AIFunctions* aiFunctions);
void FreeAIStruct(S_AIFunctions* aiFunctions);

// Fonction de chargement d'une API
int LoadAPI(char* path, S_AIFunctions* aiFunctions, int index);

// Passage des arguments du programme
E_GameMode ParseArgs(int argc, char* argv[], S_AIFunctions* aiFunctions);

#endif
