#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>

#include "../../backgammon.h"
#include "ia.h"

SMove Mouvements[4];
int flecheJouable[25];//avec case barre 24
int strategie;

void initTableauSMoveLocal(SMove tableau[4]) {
	int i;
	
	for (i=0;i<4;i++) {
		tableau[i].src_point=-1;
		tableau[i].dest_point=-1;
	}
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

int lastChecker(const SGameState * const gameState) {
	int toReturn;
	int i;
	
	i=24;
	toReturn=-1;
	
	rafraichitFlecheJouable(gameState);
	
	while (toReturn==-1 && i>=0) {
		if (flecheJouable[i]) {
			toReturn=i;
		}
		i--;
	}
	
	return toReturn;
}

int nbPionEnJeu(const SGameState * const gameState) {
	int toReturn;
	int i;
	
	toReturn=0;
	rafraichitFlecheJouable(gameState);
	
	for(i=0;i<=24;i++) {
		toReturn=toReturn+flecheJouable[i];
	}
	return toReturn;	
}


int nbBlotEnJeu(const SGameState * const gameState) {
	int toReturn;
	int i;
	
	toReturn=0;
	rafraichitFlecheJouable(gameState);
	
	for(i=0;i<24;i++) { // strictement inférieur a 24 car la barre ne compte pas
		if(flecheJouable[i]==1) {
			toReturn++;
		}
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

void calculerStrategie(const SGameState * const gameState, int lastTimeError) {
	if(lastTimeError==1) {
		strategie=0; // strategie la plus simple et la plus sûr
	} else {
		rafraichitFlecheJouable(gameState);
		if(lastChecker(gameState)<= EPos_6) {
			strategie=1; // on peu sortir un pion
		} else {
			if(calculerCoupRestant(gameState, EPlayer1)>calculerCoupRestant(gameState, EPlayer2)) { // cas ou on est en retard dans le jeu
				strategie=2;
			} else {
				strategie=3;
			}
		}
	}
}

int meilleurPlateau(SGameState* plateauATester, const SGameState * const gameState) {
	SGameState *plateauEnCours;
	int toReturn;
	
	toReturn=FALSE;
	plateauEnCours=NULL;
	effectuerLesMouvements(plateauEnCours, gameState);
	// plateauEnCours est la simulation du plateau avec les déplacements du tableau Mouvements
	
	if(calculerCoupRestant(plateauEnCours, EPlayer1)>calculerCoupRestant(plateauATester, EPlayer1)) {
		toReturn=TRUE;
	} else {
		if(strategie==0) { // assurer en prenant le premier déplacement trouvé La comparaison avec un autre coup est donc faux
				toReturn=FALSE;
		} else {
			if(strategie==1) { // privilégier la sortie d'un pion
				if(nbPionEnJeu(plateauATester)<nbPionEnJeu(plateauEnCours)){ // s'il y a moins de pions en jeu, c'est qu'il en a sortie plus.
					toReturn=TRUE;
				}
			} else {
				if(strategie==2) { // privilégier l'attaque des blots
					if(plateauATester->zones[EPos_BarP2].nb_checkers>plateauEnCours->zones[EPos_BarP2].nb_checkers) {
						toReturn=TRUE;
					}
				} else { // privilégier la défense en ne créant pas de blot
					if(nbBlotEnJeu(plateauATester)<nbBlotEnJeu(plateauEnCours)) {
						toReturn=TRUE;
					}
				}
			}
		}
	}
	return toReturn;
}

int possibleCase(const SGameState * const gameState, int source){
	return !((gameState->zones[source].player==EPlayer2)&&(gameState->zones[source].nb_checkers>1));
}

int mouvementNormalPossible(const SGameState * const gameState, int source, int distance) {
	int toReturn;
	
	if (source-distance>=0 && flecheJouable[source]>0 && possibleCase(gameState,source-distance)) {
		toReturn=TRUE;
	} else {
		toReturn=FALSE;
	}
	
	return toReturn;
}


int mouvementPossible(const SGameState * const gameState, int source, int distance) {
	int toReturn;
	int nbrPionBar;
	int dernierPion;
	
	nbrPionBar=gameState->zones[EPos_BarP1].nb_checkers;
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
			dernierPion=lastChecker(gameState);
			if (dernierPion<=EPos_6) { //si tous les pions sont dans le home
				if (distance==source+1) {//cas du nombre exact pour sortir
					toReturn=TRUE;
				} else {
					if (distance>source+1) {//si c'est le dernier pion
						if (source==dernierPion) {//si le jet est supérieur à la distance restante
							toReturn=TRUE;
						} else {//deplacement normal
							toReturn=FALSE;
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

void effectuerUnDeplacementTest(SGameState * plateau, int src_point, int de) {
	int source;
	
	if(src_point==EPos_BarP1) {
		source=25;
	} else {
		source=src_point;
	}
	
	if(source>-1){
		plateau->zones[source].nb_checkers=plateau->zones[source].nb_checkers-1;
		
		if((plateau->zones[source-de].player==EPlayer2) && (plateau->zones[source-de].nb_checkers==1)) { //cas ou on mange un pion adverse
			plateau->zones[EPos_BarP2].nb_checkers++;
			plateau->zones[source-de].player=EPlayer1;
		} else {
			if(source-de>=0) {// distance moins la valeur du dé plus la case zéro
				plateau->zones[source-de].nb_checkers++;
				plateau->zones[source-de].player=EPlayer1;
			} else {
				plateau->zones[EPos_OutP1].nb_checkers++;
			}
		}
	}
}

void effectuerLesMouvements(SGameState * plateauEnCours, const SGameState * const gameState){
	int i;
	int j;
	printf("puddi\n");
	for(j=EPos_1;j<=EPos_BarP2;j++) {
		plateauEnCours->zones[j]=gameState->zones[j];
	}
	
	for(i=0;i<4;i++) {
		if(Mouvements[i].src_point>-1) {
			plateauEnCours->zones[Mouvements[i].src_point].nb_checkers--;
			if((plateauEnCours->zones[Mouvements[i].dest_point].player==EPlayer2) && (plateauEnCours->zones[Mouvements[i].dest_point].nb_checkers==1)) {// cas ou on mange un pion adverse
				plateauEnCours->zones[EPos_BarP2].nb_checkers++;
				plateauEnCours->zones[Mouvements[i].dest_point].nb_checkers--;
			}
			plateauEnCours->zones[Mouvements[i].dest_point].nb_checkers++;
			plateauEnCours->zones[Mouvements[i].dest_point].player=EPlayer1;
		}
	}
}

void enregistrerUnDeplacement(int i, int source, int de) {
	Mouvements[i].src_point=source;
	if(source==EPos_BarP1) {
		Mouvements[i].dest_point=EPos_24-de+1;
	} else {
		if(source-de>=0) {// si le déplacement, n'est pas une sortie de pion
			Mouvements[i].dest_point=source-de;
		} else {
			Mouvements[i].dest_point=EPos_OutP1;
		}
	}
}

void decision2Move(const SGameState * const gameState) {
	int numPassage;
	
	int de1;
	int de2;
	
	int source1; // source du déplacement dans la première boucle
	int source2; // source du déplacement dans la seconde boucle
	
	SGameState plateau1;
	SGameState plateau2;
	
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
		source1=EPos_BarP1;
		while (source1>=EPos_1) {// si pas de mouvement complet et qu'on n'a pas finit de parcourir le plateau pour le permier déplacement
			if (source1!=EPos_OutP1 && mouvementPossible(gameState,source1,de1)){// un premier mouvement est possible
				plateau1=*gameState;
				mouvementPremierTrouve=TRUE;
				effectuerUnDeplacementTest(&plateau1, source1, de1);
				source2=EPos_BarP1;
				while (source2>=EPos_1) {// si pas de mouvement complet et qu'on n'a pas finit de parcourir le plateau pour le second déplacement
					if (source2!=EPos_OutP1 && mouvementPossible(&plateau1,source2,de2)){// un second déplacement est possible
						plateau2=plateau1;
						effectuerUnDeplacementTest(&plateau2, source2, de2);
						if(!mouvementCompletTrouve || meilleurPlateau(&plateau2, gameState)) {
							enregistrerUnDeplacement(0, source1, de1);
							enregistrerUnDeplacement(1, source2, de2);
							mouvementCompletTrouve=TRUE;
						}		
					}
					source2=source2-1;
				}	
			}
			if(mouvementPremierTrouve && !mouvementCompletTrouve){// cas ou 1 seul dé est jouable
				if(meilleurPlateau(&plateau1, gameState)){// si le premier déplacement est plus grand que l'ancien, il faut le préférer. C'est ce que fait meilleurPlateau
					enregistrerUnDeplacement(0, source1, de1);
				}
				mouvementPremierTrouve=FALSE;
			}
			source1=source1-1;
		}	
		numPassage++;
	}
}

void decision4Move(const SGameState * const gameState) {
	SGameState  plateau1;
	SGameState  plateau2;
	SGameState  plateau3;
	SGameState  plateau4;
	
	int mouvementCompletTrouve;
	int mouvementPremierTrouve;
	int mouvementDeuxiemeTrouve;
	int mouvementTroisiemeTrouve;
	
	int source1;
	int source2;
	int source3;
	int source4;
	
	mouvementCompletTrouve=FALSE;
	mouvementDeuxiemeTrouve=FALSE;
	mouvementTroisiemeTrouve=FALSE;
	
	source1=EPos_BarP1;
	while(!mouvementCompletTrouve && source1>=EPos_1) {
		if(source1!=EPos_OutP1 && mouvementPossible(gameState,source1,gameState->die1)) {
			plateau1= *gameState;
			mouvementPremierTrouve=TRUE;
			effectuerUnDeplacementTest(&plateau1, source1, gameState->die1);
			source2=EPos_BarP1;
			while(!mouvementCompletTrouve && source2>=EPos_1) {
				if(source2!=EPos_OutP1 && mouvementPossible(&plateau1,source2,gameState->die1)) {
					plateau2= plateau1;
					mouvementDeuxiemeTrouve=TRUE;
					effectuerUnDeplacementTest(&plateau2, source2, gameState->die1);
					source3=EPos_BarP1;
					while(!mouvementCompletTrouve && source3>=EPos_1) {
						if(source2!=EPos_OutP1 && mouvementPossible(&plateau2,source3,gameState->die1)) {
							plateau3= plateau2;
							mouvementTroisiemeTrouve=TRUE;
							effectuerUnDeplacementTest(&plateau3, source3, gameState->die1);
							source4=EPos_BarP1;
							while(!mouvementCompletTrouve && source4>=EPos_1) {
								if(source4!=EPos_OutP1 && mouvementPossible(&plateau3,source4,gameState->die1)) {
									plateau4=plateau3;
									effectuerUnDeplacementTest(&plateau4, source4, gameState->die1);
									if(!mouvementCompletTrouve || meilleurPlateau(&plateau4, gameState)) {
										enregistrerUnDeplacement(0, source1, gameState->die1);
										enregistrerUnDeplacement(1, source2, gameState->die1);
										enregistrerUnDeplacement(2, source3, gameState->die1);
										enregistrerUnDeplacement(3, source4, gameState->die1);
										mouvementCompletTrouve=TRUE;
									}
								}
								source4=source4-1;
							}
							if(!mouvementCompletTrouve && meilleurPlateau(&plateau3, gameState)) {
								enregistrerUnDeplacement(0, source1, gameState->die1);
								enregistrerUnDeplacement(1, source2, gameState->die1);
								enregistrerUnDeplacement(2, source3, gameState->die1);
							}
						}
						source3=source3-1;
					}
					if(!mouvementTroisiemeTrouve && meilleurPlateau(&plateau2, gameState)) {
						enregistrerUnDeplacement(0, source1, gameState->die1);
						enregistrerUnDeplacement(1, source2, gameState->die1);
					}
				}
				source2=source2-1;
			}
			if(!mouvementDeuxiemeTrouve && meilleurPlateau(&plateau1, gameState)) {
				enregistrerUnDeplacement(0, source1, gameState->die1);
			}
		}
		source1=source1-1;
	}
}

void redirectionEnFonctionDesDes (const SGameState * const gameState) {
	int nombreCoup;
	nombreCoup=nbMove(gameState->die1,gameState->die2);
	
	if(nombreCoup==2) {//si les deux dés sont différents et qu'on a 2 deplacements
		decision2Move(gameState);
	} else {//si on a 2 dés identiques
		decision4Move(gameState);
	}
}

int calculerCoupRestant(const SGameState * const gameState, EPlayer unJoueur) {
	int i;
	double toReturn;
	
	toReturn=0;
	for(i=EPos_1;i<=EPos_24;i++){ // on compte les pions sur le plateau
		if(gameState->zones[i].player==unJoueur) {
			if(unJoueur==EPlayer1) {
				toReturn=toReturn + (i + 1)*gameState->zones[i].nb_checkers;
			} else {
				toReturn=toReturn + ((EPos_24 -i) +1)*gameState->zones[i].nb_checkers;
			}
		}
	}
	if(unJoueur==EPlayer1) { // on ajoute les pions coincés dans la barre
		toReturn=toReturn + 25*gameState->zones[EPos_BarP1].nb_checkers;
	} else {
		toReturn=toReturn + 25*gameState->zones[EPos_BarP2].nb_checkers;
	}
	return toReturn;
}

int pratiquementGagner(double deplacementRestantP1, double deplacementRestantP2) {
	int toReturn;
	
	if((0.75*deplacementRestantP1) >= deplacementRestantP2) {
		toReturn=1;
	} else {
		toReturn=0;
	}
	return toReturn;
}

void InitLibrary(char name[50]) {

}

void StartMatch(const unsigned int target_score) {

}

void StartGame() {
	strategie=0;
}

void EndGame() {

}

int DoubleStack(const SGameState * const gameState) {	
	return pratiquementGagner(calculerCoupRestant(gameState, EPlayer1), calculerCoupRestant(gameState, EPlayer2));
}

int TakeDouble(const SGameState * const gameState) {	
	return !pratiquementGagner(calculerCoupRestant(gameState, EPlayer2),calculerCoupRestant(gameState, EPlayer1));
}

void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError) {
	rafraichitFlecheJouable(gameState);
	initTableauSMoveLocal(Mouvements);
	calculerStrategie(gameState, lastTimeError);
	
	redirectionEnFonctionDesDes(gameState);

	copierTableauSMove(Mouvements,moves);
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

void effectuerUnDeplacement(SGameState * plateau, int source, int destination) {
	if(source>=0) {
		plateau->zones[source].nb_checkers=plateau->zones[source].nb_checkers-1;
		plateau->zones[destination].player=EPlayer1;
		plateau->zones[destination].nb_checkers=plateau->zones[destination].nb_checkers+1;
	}
}



int main (int argc, char *argv[]) {
	SGameState theGame;
	SMove moveTab[4];
	int i, j, k, error;
	
	initGameBoard(theGame.zones);
	theGame.score=0;
	theGame.scoreP2=0;
	theGame.stake=1;
	srand(time(NULL)); //initialisation de la fonction de randomisation
	
	
	while (theGame.zones[EPos_OutP1].nb_checkers<15) {
	theGame.die1=(rand()%6)+1;	// die1
	theGame.die2=(rand()%6)+1;	// die2
	
	for(j=0;j<4;j++) {
		moveTab[j].src_point=-1;
		moveTab[j].dest_point=-1;
	}
	printf("de1 : %i de2 : %i\n", theGame.die1, theGame.die2);	
	MakeDecision(&theGame, moveTab, error);
	for(i=0;i<4;i++) {
		printf("deplacement %i : %i --> %i\n", i, moveTab[i].src_point, moveTab[i].dest_point);	
		effectuerUnDeplacement(&theGame, moveTab[i].src_point, moveTab[i].dest_point);
	}
	
	
	
		for(k=0;k<28;k++){
			if (theGame.zones[k].nb_checkers!=0){
				printf("%i - joueur %i : %i\n", k, theGame.zones[k].player, theGame.zones[k].nb_checkers);
			} else {
				printf("%i - none\n", k);
			}
		}
	}
	/*//affichage
	printf("%i\n",lastChecker());
	*/
	return 0;
}
