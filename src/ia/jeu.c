# include <stdlib.h>
# include <stdio.h>
# include <time.h>

# include "../../backgammon.h"
# include "jeu.h"



int min(int a, int b){
	int Result;

	if(a<b){
		Result=a;
	} else {
		Result=b;
	}

	return Result;
}

int max(int a, int b){
	int Result;

	if(a>b){
		Result=a;
	} else {
		Result=b;
	}

	return Result;
}

int nbMove(int a, int b){
	int Result;

	if(a==b){
		Result=4;
	} else {
		Result=2;
	}

	return Result;
}

int distanceToWin(const SZone* zones){
	int Result, i;

	Result=0;

	for(i=EPos_1;i<=EPos_24;i++){
		if(zones[i].player==EPlayer1){
			// i+1 est le nombre de déplacement restant pour sortir le pion
			Result=Result+((i+1)*zones[i].nb_checkers);
		}
	}

	return Result;
}

EPosition LastChecker1(const SZone* zones){
	EPosition Result, position;

	Result=EPos_OutP1;

	if(zones[EPos_BarP1].nb_checkers>0){
		Result=EPos_BarP1;
	} else {
		position=EPos_24;

		// tant que Resultat n'a pas changé et qu'on est dans une case de 1 à 24
		while ((Result==EPos_OutP1)&&(position>=EPos_1)) {
			if((zones[position].player==EPlayer1)&&(zones[position].nb_checkers>0)){
				Result=position;
			} else {
				position=position-1;
			}
		}

	}

	return Result;
}

//version numéro2 de LastChecker avec un paramètre N, Nieme dernier pion
EPosition LastChecker2(const SZone* zones, int N){
	EPosition Result, position;
	int k;

	//initialisation dans le cas ou tous les pions sont sortis
	Result=EPos_OutP1;
	k=N;

	//S'il y a des pion sur la barre
	k=k-zones[EPos_BarP1].nb_checkers;

	if(k<1){
		Result=EPos_BarP1;
	} else {
		//on se place sur la case 24 et on peut aller jusqu'a la 1
		position=EPos_24;

		// tant que Resultat n'a pas changé et qu'on est dans une case de 1 à 24
		while ((Result==EPos_OutP1)&&(position>=EPos_1)){
			if(zones[position].player==EPlayer1){
				k=k-zones[position].nb_checkers;
				if(k<1){
				// condition de sortie de la boucle
				Result=position;
				}
			}
			position=position-1;
		}
	}

	return Result;
}

int possibleCase(SZone arrow){
	return !((arrow.player==EPlayer2)&&(arrow.nb_checkers>1));
}

double strikeJet(int distance){

	switch(distance){
	case 1:
		return 30.56;
		break;
	case 2:
		return 33.33;
		break;
	case 3:
		return 38.89;
		break;
	case 4:
		return 41.67;
		break;
	case 5:
		return 41.67;
		break;
	case 6:
		return 47.22;
		break;
	case 7:
		return 16.67;
		break;
	case 8:
		return 16.67;
		break;
	case 9:
		return 13.89;
		break;
	case 10:
		return 8.33;
		break;
	case 11:
		return 5.56;
		break;
	case 12:
		return 8.33;
		break;
	case 15:
		return 2.78;
		break;
	case 16:
		return 2.78;
		break;
	case 18:
		return 2.78;
		break;
	case 20:
		return 2.78;
		break;
	case 24:
		return 2.78;
		break;
	default:
		return 0;
		break;
	}

}
// fonction qui évalue la probabilité de se faire manger si l'on bouge un pion vers "position"
double probabilityOfBeEaten(const SZone* zones, EPosition position){
	double Result;
	int i;

	Result=0;

	if((zones[position].player==EPlayer1)&&(zones[position].nb_checkers==0)){
		for(i=position-1;i>=EPos_1;i--){
			if((zones[i].player==EPlayer2)&&(zones[i].nb_checkers>0)){
				Result=max(Result, strikeJet(position-i));
			}
		}
	}

	return Result;
}

void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError){
	/*Technique du "je fais avancer le dernier pion"*/
	EPosition lastPosition;
	int i, j, m, impossible;

	// i sera le Nieme dernier pion. On l'initialise à 1 qui correspond au dernier
	i=1;
	// j est l'index des coups à jouer, 0 à l'initialisation
	j=0;
	// m reçoit la nombre de déplacement a effectuer
	m=nbMove(gameState->die1, gameState->die2);
	// impossible est à 1 s'il n'est pas possible d'effectuer d'autres déplacements
	impossible=0;

	// tant qu'il reste des déplacements à effectuer
	while((j<m)&&!(impossible)){

		lastPosition=LastChecker2(gameState->zones, i);
		// cas ou le dernier pion est sur la barre
		if(lastPosition==EPos_BarP1){
			// On cherche d'abord a effectuer le plus grand déplacement
			if(possibleCase(gameState->zones[lastPosition-max(gameState->die1, gameState->die2)])){
				moves[j].src_point=lastPosition;
				moves[j].dest_point=lastPosition-max(gameState->die1, gameState->die2);
				j++;
				i=1;
			// sinon on cherche a effectuer le plus petit déplacement
			} else if(possibleCase(gameState->zones[lastPosition-min(gameState->die1, gameState->die2)])){
				moves[j].src_point=lastPosition;
				moves[j].dest_point=lastPosition-min(gameState->die1, gameState->die2);
				j++;
				i=1;
			} else {
				// sinon c'est que le déplacement est impossible et on ne peut plus jouer
				impossible=1;
			}
		} else {

		}
	}


}

void initGameBoard(SZone* gameBoard){
	SZone arrow; // une flèche du plateau;
	int i;

	arrow.player=EPlayer2;
	arrow.nb_checkers=0;
	for(i=0;i<28;i++){
		gameBoard[i]=arrow;
	}
	// Disposition des pions du joueur2
	arrow.player=EPlayer2;
	arrow.nb_checkers=2;
	gameBoard[EPos_1]=arrow;

	arrow.nb_checkers=5;
	gameBoard[EPos_12]=arrow;
	gameBoard[EPos_19]=arrow;

	arrow.nb_checkers=3;
	gameBoard[EPos_17]=arrow;

	// Disposition des pions du joueur1
	arrow.player=EPlayer1;
	arrow.nb_checkers=2;
	gameBoard[EPos_24]=arrow;

	arrow.nb_checkers=5;
	gameBoard[EPos_13]=arrow;
	gameBoard[EPos_6]=arrow;

	arrow.nb_checkers=3;
	gameBoard[EPos_8]=arrow;
}


int main (int argc, char *argv[])
{
	SGameState theGame;
	SMove moveTab[4];
	int i, error;

	initGameBoard(theGame.zones);
	theGame.score=0;
	theGame.scoreP2=0;
	theGame.stake=1;
	srand(time(NULL)); //initialisation de la fonction de randomisation


	//while ((theGame.zones[EPos_OutP1].nb_checkers<15) &&
	//		(theGame.zones[EPos_OutP2].nb_checkers<15)) {
		theGame.die1=(rand()%6)+1;	// die1
		theGame.die2=(rand()%6)+1;	// die2


		//MakeDecision(&theGame, moveTab, error);
	//}
	for(i=0;i<28;i++){
		if (theGame.zones[i].nb_checkers!=0){
			printf("%i - joueur %i : %i\n", i, theGame.zones[i].player, theGame.zones[i].nb_checkers);
		} else {
				printf("%i - none\n", i);
		}
	}

	return 0;
}
