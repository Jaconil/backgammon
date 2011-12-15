#include "backgammon.h"
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////
// Dans la librairie
//

void InitLibrary(char name[50])
{
	printf("InitLibrary\n");
	strcpy(name,"Fabien Picarougne");
}

void StartMatch(const unsigned int target_score)
{
	printf("StartMatch\n");
}

void StartGame()
{
	printf("StartGame\n");
}

void EndGame()
{
	printf("EndGame\n");
}

void EndMatch()
{
	printf("EndMatch\n");
}

int DoubleStack(const SGameState * const gameState)
{
	printf("DoubleStack\n");
	return(0);
}

int TakeDouble(const SGameState * const gameState)
{
	printf("TakeDouble\n");
	return(0);
}

void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError)
{
	printf("MakeDecision\n");
}


//////////////////////////////////////////////////////////
// Dans l'exŽcutable
//

void deroulement_du_jeu()	
//int main()
{
	//*****// ˆ chaque utilisation de gameState, ne pas oublier de faire une copie de tous les ŽlŽments (pas fait ici)
	SGameState gameState;
	SMove moves[4];

	//*****// ˆ faire pour chaque joueur
	
	// Chargement de la librairie (chargement des pointeurs de fonctions des fonctions dŽcrites dans "backgammon.h")
	// -> ˆ ermplacer par votre code de chargement
	pfInitLibrary j1InitLibrary=InitLibrary;
	pfStartMatch j1StartMatch=StartMatch;
	pfStartGame j1StartGame=StartGame;
	pfEndGame j1EndGame=EndGame;
	pfEndMatch j1EndMatch=EndMatch;
	pfDoubleStack j1DoubleStack=DoubleStack;
	pfTakeDouble j1TakeDouble=TakeDouble;
	pfMakeDecision j1MakeDecision=MakeDecision;
	// ...
	pfTakeDouble j2TakeDouble=TakeDouble;

	// Initialisation de la librairie
	char name[50];
	j1InitLibrary(name);
	j1StartMatch(5);

		//*****// ˆ faire pour chaque jeu
		j1StartGame();
			//*****// pour chaque joueur, tant que ce n'est pas fini
			if (j1DoubleStack(&gameState))
				j2TakeDouble(&gameState);
			j1MakeDecision(&gameState,moves,0);
		j1EndGame();

	j1EndMatch();

	return(0);
}
