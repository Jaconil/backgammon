#include <stdio.h>
#include <dlfcn.h>

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
        aiFunctions->ai[i] = NULL;
        aiFunctions->AI_InitLibrary[i] = NULL;
        aiFunctions->AI_StartMatch[i] = NULL;
        aiFunctions->AI_StartGame[i] = NULL;
        aiFunctions->AI_EndGame[i] = NULL;
        aiFunctions->AI_EndMatch[i] = NULL;
        aiFunctions->AI_DoubleStack[i] = NULL;
        aiFunctions->AI_TakeDouble[i] = NULL;
        aiFunctions->AI_MakeDecision[i] = NULL;
    }
}

/* Procédure qui libere les ressources utilisees par les API
 * @param S_AIFunctions* aiFunctions
 *     Structure de stockage des fonctions des bibliotheques
 */
void FreeAIStruct(S_AIFunctions* aiFunctions)
{
    if (aiFunctions->ai[0] != NULL)
        dlclose(aiFunctions->ai[0]);
        
    if (aiFunctions->ai[1] != NULL)
        dlclose(aiFunctions->ai[1]);
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
	if ((aiFunctions->ai[index] = dlopen(path, RTLD_LAZY)) == NULL)
	{
		fprintf(stderr, "Erreur de chargement de l'API %s\n", path);
		return 0;
	}
        
    if ((aiFunctions->AI_InitLibrary[index] = (pfInitLibrary)dlsym(aiFunctions->ai[index], "InitLibrary")) == NULL) return 0;
    if ((aiFunctions->AI_StartMatch[index] = (pfStartMatch)dlsym(aiFunctions->ai[index], "StartMatch")) == NULL) return 0;    
    if ((aiFunctions->AI_StartGame[index] = (pfStartGame)dlsym(aiFunctions->ai[index], "StartGame")) == NULL) return 0;   
    if ((aiFunctions->AI_EndGame[index] = (pfEndGame)dlsym(aiFunctions->ai[index], "EndGame")) == NULL) return 0;       
    if ((aiFunctions->AI_EndMatch[index] = (pfEndMatch)dlsym(aiFunctions->ai[index], "EndMatch")) == NULL) return 0;   
    if ((aiFunctions->AI_DoubleStack[index] = (pfDoubleStack)dlsym(aiFunctions->ai[index], "DoubleStack")) == NULL) return 0;   
    if ((aiFunctions->AI_TakeDouble[index] = (pfTakeDouble)dlsym(aiFunctions->ai[index], "TakeDouble")) == NULL) return 0;   
    if ((aiFunctions->AI_MakeDecision[index] = (pfMakeDecision)dlsym(aiFunctions->ai[index], "MakeDecision")) == NULL) return 0;   

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
        if (LoadAPI(argv[1], aiFunctions, 0))
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
