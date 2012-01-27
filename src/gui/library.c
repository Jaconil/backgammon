#include <stdio.h>

// Compatibilité Windows
#ifdef _WIN32
    #include <SDL/SDL_loadso.h>
    #define DLOPEN(path) SDL_LoadObject(path)
    #define DLSYM SDL_LoadFunction
    #define DLCLOSE SDL_UnloadObject
#else
    #include <dlfcn.h>
    #define DLOPEN(path) dlopen(path, RTLD_LAZY)
    #define DLSYM dlsym
    #define DLCLOSE dlclose
#endif

#include "structures.h"
#include "library.h"

/* Procédure qui initialise la structure de stockage des fonctions de l'API
 * @param S_AIFunctions* aiFunctions
 *     Structure de stockage des fonctions des bibliotheques
 */
void InitAIStruct(S_AIFunctions* aiFunctions)
{
    int i = 0;

    for (i=0; i<2; i++)
    {
        aiFunctions[i].ai = NULL;
        aiFunctions[i].AI_InitLibrary = NULL;
        aiFunctions[i].AI_StartMatch = NULL;
        aiFunctions[i].AI_StartGame = NULL;
        aiFunctions[i].AI_EndGame = NULL;
        aiFunctions[i].AI_EndMatch = NULL;
        aiFunctions[i].AI_DoubleStack = NULL;
        aiFunctions[i].AI_TakeDouble = NULL;
        aiFunctions[i].AI_MakeDecision = NULL;
    }
}

/* Procédure qui libere les ressources utilisees par les API
 * @param S_AIFunctions* aiFunctions
 *     Structure de stockage des fonctions des bibliotheques
 */
void FreeAIStruct(S_AIFunctions* aiFunctions)
{
    if (aiFunctions[0].ai != NULL)
        DLCLOSE(aiFunctions[0].ai);

    if (aiFunctions[1].ai != NULL)
        DLCLOSE(aiFunctions[1].ai);
}

/* Fonction de chargement d'une API
 * @param char* path
 *     Chemin vers la bibliotheque
 * @param S_AIFunctions* aiFunctions
 *     Structure de stockage des fonctions des bibliotheques
 * @param int index
 *     Indice courant dans la structure
 * @return int
 *     1 si aucune erreur, 0 sinon
 */
int LoadAPI(char* path, S_AIFunctions* aiFunctions, int index)
{
    if ((aiFunctions[index].ai = DLOPEN(path)) == NULL)
	{
		fprintf(stderr, "Erreur de chargement de l'API %s\n", path);
		return 0;
	}

    if ((aiFunctions[index].AI_InitLibrary = (pfInitLibrary)DLSYM(aiFunctions[index].ai, "InitLibrary")) == NULL) return 0;
    if ((aiFunctions[index].AI_StartMatch = (pfStartMatch)DLSYM(aiFunctions[index].ai, "StartMatch")) == NULL) return 0;
    if ((aiFunctions[index].AI_StartGame = (pfStartGame)DLSYM(aiFunctions[index].ai, "StartGame")) == NULL) return 0;
    if ((aiFunctions[index].AI_EndGame = (pfEndGame)DLSYM(aiFunctions[index].ai, "EndGame")) == NULL) return 0;
    if ((aiFunctions[index].AI_EndMatch = (pfEndMatch)DLSYM(aiFunctions[index].ai, "EndMatch")) == NULL) return 0;
    if ((aiFunctions[index].AI_DoubleStack = (pfDoubleStack)DLSYM(aiFunctions[index].ai, "DoubleStack")) == NULL) return 0;
    if ((aiFunctions[index].AI_TakeDouble = (pfTakeDouble)DLSYM(aiFunctions[index].ai, "TakeDouble")) == NULL) return 0;
    if ((aiFunctions[index].AI_MakeDecision = (pfMakeDecision)DLSYM(aiFunctions[index].ai, "MakeDecision")) == NULL) return 0;

    return 1;
}

/* Fonction qui examine les arguments pour determiner le mode de jeu
 * @param int argc
 *     Nombre d'arguments
 * @param char* argv[]
 *     Liste des arguments
 * @param S_AIFunctions* aiFunctions
 *     Structure de stockage des fonctions des bibliotheques
 * @return E_GameMode
 *     Mode de jeu du programme
 */
E_GameMode ParseArgs(int argc, char* argv[], S_AIFunctions* aiFunctions)
{
    E_GameMode returnValue = ERROR;

    if (argc == 1)
        returnValue = HUMAN_HUMAN;
    else if (argc == 2)
    {
        if (LoadAPI(argv[1], aiFunctions, 1))
            returnValue = HUMAN_AI;
    }
    else if (argc == 3)
    {
        if (LoadAPI(argv[1], aiFunctions, 0) && LoadAPI(argv[2], aiFunctions, 1))
            returnValue = AI_AI;
    }
    else
        fprintf(stderr, "Nombre d'arguments incorrects. L'utilisation du programme est detaillée dans le fichier README.\n");

    return returnValue;
}
