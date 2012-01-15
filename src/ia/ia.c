#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>

#include "../../backgammon.h"
#include "ia.h"

SMove MouvementsPrecedant[4];
SMove Mouvements[4];
int flecheJouable[25];//avec case barre 24

void initTableauSMoveLocal(SMove tableau[4]) {
	int i;
	
	for (i=0;i<4;i++) {
		tableau[i].src_point=-1;
		tableau[i].dest_point=-1;
	}
}

void StartGame() {
	initTableauSMoveLocal(MouvementsPrecedant);
}

void rafraichitFlecheJouable(const SGameState * const gameState) {
	int i;
	
	for (i=0;i<24;i++) {
		if (gameState->zones[i].player==EPlayer1) {
			flecheJouable[i]=gameState->zones[i].nb_checkers;
		} else {
			flecheJouable[i]=0;
		}
	}
	flecheJouable[24]=gameState->zones[EPos_BarP1].nb_checkers;
}

void copierTableauSMove(SMove source[4], SMove destination[4]) {
	memcpy(destination,source,sizeof(source)*4);
}

int lastChecker() {
	int toReturn;
	int i;
	
	i=24;
	toReturn=-1;
	
	while (toReturn==-1 && i>=0) {
		if (flecheJouable[i]) {
			toReturn=i;
		}
		i--;
	}
	
	return toReturn;
}

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

int nombrePionBar(const SGameState * const gameState) {
	return gameState->zones[EPos_BarP1].nb_checkers;
}

int distanceToWin(const SGameState * const gameState){
	int Result, i;
	int nombrePionsPrisonniers;
	
	Result=0;
	
	for(i=EPos_1;i<=EPos_24;i++){
		if(gameState->zones[i].player==EPlayer1){
			// i+1 est le nombre de déplacement restant pour sortir le pion
			Result=Result+((i+1)*gameState->zones[i].nb_checkers);
		}
	}
	nombrePionsPrisonniers=nombrePionBar(gameState);
	
	Result=Result+(nombrePionsPrisonniers*25);
	
	return Result;
}

int possibleCase(const SGameState * const gameState, EPosition source){
	return !((gameState->zones[source].player==EPlayer2)&&(gameState->zones[source].nb_checkers>1));
}

int mouvementNormalPossible(const SGameState * const gameState, EPosition source, int distance) {
	int toReturn;
	
	if (flecheJouable[source]>0 && possibleCase(gameState,source-distance)) {
		toReturn=TRUE;
	} else {
		toReturn=FALSE;
	}
	
	return toReturn;
}

int mouvementPossible(const SGameState * const gameState, EPosition source, int distance) {
	int toReturn;
	int nbrPionBar;
	int dernierPion;
	
	nbrPionBar=nombrePionBar(gameState);
	rafraichitFlecheJouable(gameState);

	if ((distance==gameState->die1 || distance==gameState->die2) && flecheJouable[source]) {
		if(nbrPionBar>0){//si un pion est prisonnier
			if (source==EPos_BarP1) {//si la source est bien un prisonnier
				if (possibleCase(gameState,(EPos_24-distance)+1)) {//si la case dest est libre
					toReturn=TRUE;
				} else {
					toReturn=FALSE;
				}
			} else {
				toReturn=FALSE;
			}
		} else {
			dernierPion=lastChecker();
			if (dernierPion<=EPos_6) { //si tous les pions sont dans le home
				if (distance==source) {//cas du nombre exact pour sortir
					toReturn=TRUE;
				} else {
					if (source==dernierPion) {//si c'est le dernier pion
						if (distance>source) {//si le jet est supérieur à la distance restante
							toReturn=TRUE;
						} else {//deplacement normal
							toReturn=mouvementNormalPossible(gameState,source,distance);
						}
					} else {//deplacement normal
						toReturn=mouvementNormalPossible(gameState,source,distance);
					}
				}
			} else {//deplacement normal
				toReturn=mouvementNormalPossible(gameState,source,distance);
			}
		}
	} else {
		toReturn=FALSE;
	}
	
	return toReturn;
}

void effectuerUnDeplacementTest(SGameState * plateau, EPosition source, int de) {
	plateau->zones[source].nb_checkers=plateau->zones[source].nb_checkers-1;
	if(source==EPos_BarP1) {
		plateau->zones[25-de].nb_checkers=plateau->zones[25-de].nb_checkers+1;
		plateau->zones[25-de].player=EPlayer1;
	} else {
		if(source-de>=0) {
			plateau->zones[source-de].nb_checkers=plateau->zones[source-de].nb_checkers+1;
			plateau->zones[source-de].player=EPlayer1;
		} else {
			plateau->zones[EPos_OutP1].nb_checkers=plateau->zones[EPos_OutP1].nb_checkers+1;
		}
	}
}

void enregistrerUnDeplacement(SMove deplacement, EPosition source, int de) {
	deplacement.src_point=source;
	if(source-de>=0) {// si le déplacement, n'est pas une sortie de pion
		deplacement.dest_point=source-1;
	} else {
		deplacement.dest_point=EPos_OutP1;
	}
}

void decision2Move(const SGameState * const gameState) {
	int numPassage;
	
	int de1;
	int de2;
	
	int Source1; // source du déplacement dans la première boucle
	int Source2; // source du déplacement dans la seconde boucle
	
	SGameState * plateau1;
	
	int mouvementCompletTrouve;
	int mouvementPremierTrouve;
	
	mouvementCompletTrouve=FALSE;
	mouvementPremierTrouve=FALSE;
	numPassage=1;
		
	while (!mouvementCompletTrouve && numPassage<=2) {	
		if (numPassage==1) {//le premier dé utilisé est le dé 1
			de1=gameState->die1;
			de2=gameState->die2;
		} else {
			de1=gameState->die2;
			de2=gameState->die1;
		}
		Source1=EPos_BarP1;
		while (!mouvementCompletTrouve && Source1>=EPos_1) {// si pas de mouvement complet et qu'on n'a pas finit de parcourir le plateau pour le permier déplacement
			if (Source1!=EPos_OutP1 && mouvementPossible(gameState,Source1,de1)){// un premier mouvement est possible
				memcpy(&plateau1,&gameState,sizeof(gameState));
				mouvementPremierTrouve=TRUE;
				effectuerUnDeplacementTest(plateau1, Source1, de1);
				Source2=EPos_BarP1;
				while (!mouvementCompletTrouve && (Source2>=EPos_1)) {// si pas de mouvement complet et qu'on n'a pas finit de parcourir le plateau pour le second déplacement
					if (Source2!=EPos_OutP1 && mouvementPossible(plateau1,Source2,de2)){// un second déplacement est possible
						enregistrerUnDeplacement(Mouvements[0],Source1, de1);
						enregistrerUnDeplacement(Mouvements[1], Source2, de2);
						mouvementCompletTrouve=TRUE;
					}
					Source2=Source2-1;
				}	
			}
			if(mouvementPremierTrouve && !mouvementCompletTrouve){// cas ou 1 seul dé est jouable
				if((Mouvements[0].dest_point - Mouvements[0].src_point)<de1){// si le premier déplacement est plus grand que l'ancien, il faut le préférer.
					enregistrerUnDeplacement(Mouvements[0], Source1, de1);
				}
				mouvementPremierTrouve=FALSE;
			}
			Source1=Source1-1;
		}	
		numPassage++;
	}
}

void redirectionEnFonctionDesDes (const SGameState * const gameState) {
	int nombreCoup;
	nombreCoup=nbMove(gameState->die1,gameState->die2);
	
	if(nombreCoup==2) {//si les deux dés sont différents et qu'on a 2 deplacements
		decision2Move(gameState);
	} else {//si on a 2 dés identiques
	}
}

void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError) {
	rafraichitFlecheJouable(gameState);
	initTableauSMoveLocal(Mouvements);
	
	//redirectionEnFonctionDesDes(gameState);
	decision2Move(gameState);

	copierTableauSMove(Mouvements,moves);
	copierTableauSMove(moves, MouvementsPrecedant);
}



/********** fonction de test ***********/
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



int main (int argc, char *argv[]) {
	SGameState theGame;
	SMove moveTab[4];
	int i, j, error;
	
	initGameBoard(theGame.zones);
	theGame.score=0;
	theGame.scoreP2=0;
	theGame.stake=1;
	srand(time(NULL)); //initialisation de la fonction de randomisation
	
	
	//while ((theGame.zones[EPos_OutP1].nb_checkers<15) &&
	//		(theGame.zones[EPos_OutP2].nb_checkers<15)) {
	theGame.die1=(rand()%6)+1;	// die1
	theGame.die2=(rand()%6)+1;	// die2
	
	for(j=0;j<4;j++) {
		moveTab[j].src_point=-1;
		moveTab[j].dest_point=-1;
	}
	printf("de1 : %i de2 : %i\n", theGame.die1, theGame.die2);	
	MakeDecision(&theGame, moveTab, error);
	printf("recoucou\n");	
	//}
	
	/*//affichage
	for(i=0;i<28;i++){
		if (theGame.zones[i].nb_checkers!=0){
			printf("%i - joueur %i : %i\n", i, theGame.zones[i].player, theGame.zones[i].nb_checkers);
		} else {
			printf("%i - none\n", i);
		}
	}
	
	
	printf("%i\n",lastChecker());
	*/
	return 0;
}
