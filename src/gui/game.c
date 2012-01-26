#include <stdio.h>
#include <stdlib.h>

#include "cste.h"
#include "structures.h"
#include "game.h"

/* Procedure qui lance les des aleatoirement
 * @param S_GameState* gameState
 *     Etat du jeu
 */
void RollDice(S_GameState* gameState)
{
    gameState->die1 = (rand() % 6) + 1;
    gameState->die2 = (rand() % 6) + 1;

    // Changement d'etat
    if (gameState->currentStage == WAITING_FIRST_ROLL)
    {
        while (gameState->die1 == gameState->die2)
        {
            gameState->die1 = (rand() % 6) + 1;
            gameState->die2 = (rand() % 6) + 1;
        }

        if (gameState->die1 > gameState->die2)
            gameState->currentPlayer = EPlayer1;
        else
            gameState->currentPlayer = EPlayer2;

        gameState->currentStage = FIRST_ROLL_POPUP;
    }

    if (gameState->die1 == gameState->die2)
    {
        gameState->useDie1 = 2;
        gameState->useDie2 = 2;
    }
    else
    {
        gameState->useDie1 = 1;
        gameState->useDie2 = 1;
    }
}

/* Fonction qui indique si la zone cliquee est valide comme source
 * @param int zone
 *     Zone cliquee
 * @param S_GameState* gameState
 *     Etat du jeu
 * @return int
 *     1 si la zone cliquee est valide, 0 sinon
 */
int IsValidSrc(int zone, S_GameState* gameState)
{
    return (zone != -1 &&
            zone != EPos_OutP1 &&
            zone != EPos_OutP2 &&
            gameState->zones[zone].nb_checkers > 0 &&
            gameState->zones[zone].player == gameState->currentPlayer);
}

/* Fonction qui indique si la zone cliquee est valide comme destination
 * @param int zone
 *     Zone cliquee
 * @param S_GameState* gameState
 *     Etat du jeu
 * @return int
 *     1 si la zone cliquee est valide, 0 sinon
 */
int IsValidDst(int zone, S_GameState* gameState)
{
    if (zone == -1 || zone == EPos_BarP1 || zone == EPos_BarP2)
        return 0;

    int valid = 0;
    int bearoff = CanBearOff(gameState);

    // On regarde si la fleche est valide
    int src = (int)(gameState->currentZone);
    int dest = zone;

    if (gameState->currentPlayer == EPlayer1)
    {
        // Repositionnement de la sortie et de la barre pour les calculs
        if (zone == EPos_OutP1)
            dest = bearoff ? -1 : 99;

        if (gameState->currentZone == EPos_BarP1)
            src = 24;

        printf("src = %i, dest = %i\n", src, dest);

        if (gameState->currentZone == EPos_BarP1 || gameState->zones[EPos_BarP1].nb_checkers == 0)
        {
            if ((src - gameState->die1 == dest && gameState->useDie1 > 0) ||
                (src - gameState->die2 == dest && gameState->useDie2 > 0))
                valid = 1;

            if (bearoff && !valid && dest == -1)
            {
                if (((src - (int)gameState->die1) < dest && gameState->useDie1 > 0) ||
                    ((src - (int)gameState->die2) < dest && gameState->useDie2 > 0))
                {
                    // On regarde s'il y a des dames sur les fleches superieures
                    int sum = 0;
                    int i;

                    for (i=src+1; i<6; i++)
                    {
                        if (gameState->zones[i].nb_checkers > 0 && gameState->zones[i].player == EPlayer1)
                            sum += gameState->zones[i].nb_checkers;
                    }

                    if (sum == 0)
                        valid = 1;
                }
            }
        }


    }
    else
    {
        // Repositionnement de la sortie et de la barre pour les calculs
        if (zone == EPos_OutP2)
            dest = bearoff ? 24 : 99;

        if (gameState->currentZone == EPos_BarP2)
            src = -1;

        if (gameState->currentZone == EPos_BarP2 || gameState->zones[EPos_BarP2].nb_checkers == 0)
        {
            if ((src + gameState->die1 == dest && gameState->useDie1 > 0) ||
               (src + gameState->die2 == dest && gameState->useDie2 > 0))
                valid = 1;

            if (bearoff && !valid)
            {
                if ((src + (int)gameState->die1 > dest && gameState->useDie1 > 0) ||
                    (src + (int)gameState->die2 > dest && gameState->useDie2 > 0))
                {
                    // On regarde s'il y a des dames sur les fleches superieures
                    int sum = 0;
                    int i;

                    for (i=src+1; i<24; i++)
                    {
                        if (gameState->zones[i].nb_checkers > 0 && gameState->zones[i].player == EPlayer2)
                            sum += gameState->zones[i].nb_checkers;
                    }

                    if (sum == 0)
                        valid = 1;
                }
            }
        }
    }

    // Validation du nombre de pions sur la fleche de destination
    valid &= (gameState->zones[zone].nb_checkers <= 1 ||
             gameState->zones[zone].player == gameState->currentPlayer);

    return valid;
}

/* Fonction qui effectue le mouvement
 * @param int zone
 *     Zone cliquee
 * @param S_GameState* gameState
 *     Etat du jeu
 */
void DoMove(int zone, S_GameState* gameState)
{
    // Gestion d'un blot
    if (gameState->zones[zone].nb_checkers == 1 && gameState->zones[zone].player != gameState->currentPlayer)
    {
        gameState->zones[zone].nb_checkers--;

        if (gameState->currentPlayer == EPlayer1)
            gameState->zones[EPos_BarP2].nb_checkers++;
        else
            gameState->zones[EPos_BarP1].nb_checkers++;
    }

    // Changement de couleur de la fleche
    gameState->zones[gameState->currentZone].nb_checkers--;
    gameState->zones[zone].nb_checkers++;
    gameState->zones[zone].player = gameState->currentPlayer;

    // Gestion des utilisations des des
    int src = gameState->currentZone;

    // Repositionnement de la sortie et de la barre pour les calculs
    if (gameState->currentPlayer == EPlayer1)
    {
        if (zone == EPos_OutP1)                       zone = -1;
        if (gameState->currentZone == EPos_BarP1)     src = 24;
    }
    else
    {
        if (zone == EPos_OutP2)                       zone = 24;
        if (gameState->currentZone == EPos_BarP2)     src = -1;
    }

    int diff = abs(src - zone);

    if (gameState->die1 == diff && gameState->useDie1 > 0)
        gameState->useDie1--;
    else if (gameState->die2 == diff && gameState->useDie2 > 0)
        gameState->useDie2--;
    else
    {
        // Cas de sortie ou on peut sortir une dame sur une fleche plus petite
        if (gameState->useDie1 > 0 && gameState->useDie2 > 0)
        {
            if (gameState->die1 > gameState->die2)
                gameState->useDie1--;
            else
                gameState->useDie2--;
        }
        else if (gameState->useDie1 > 0)
            gameState->useDie1--;
        else
            gameState->useDie2--;
    }

    gameState->currentZone = -1;

    // Gestion du changement de transition
    if (gameState->useDie1 == 0 && gameState->useDie2 == 0)
    {
        if (IsFinish(gameState))
            gameState->currentStage = FINISH_POPUP;
        else
        {
            if (gameState->currentPlayer == EPlayer1)
                gameState->currentPlayer = EPlayer2;
            else
                gameState->currentPlayer = EPlayer1;

            if (gameState->cubeOwner == gameState->currentPlayer || gameState->stake == 1)
                gameState->currentStage = WAITING_ROLL_DBL;
        	else
        		gameState->currentStage = WAITING_ROLL;
        }
    }
}

/* Fonction qui determine si des mouvements sont possibles avec les des
 * @param S_GameState* gameState
 *     Etat du jeu
 * @return int
 *     1 si des mouvements sont possibles, 0 sinon
 */
int IsPossibleMove(S_GameState* gameState)
{
    int possible = 0;
    int i = 0;

    while (!possible && i < 28)
    {
        if (IsValidSrc(i, gameState))
        {
            int currentZone = gameState->currentZone;
            gameState->currentZone = i;

            int j = 0;

            while (!possible && j < 28)
            {
                printf("pos src=%i, dest=%i\n", gameState->currentZone, j);
                if (IsValidDst(j, gameState))
                    possible = 1;

                j++;
            }

            gameState->currentZone = currentZone;
        }

        i++;
    }

    printf("ok, possible = %i\n", possible);

    return possible;
}

/* Fonction qui determine si le joueur courant peut sortir ses dames
 * @param S_GameState* gameState
 *     Etat du jeu
 * @return int
 *     1 s'il peut sortir ses dames, 0 sinon
 */
int CanBearOff(S_GameState* gameState)
{
    int i;
    int sum = 0;

    if (gameState->currentPlayer == EPlayer1)
    {
        for (i=0; i<6; i++)
        {
            if (gameState->zones[i].nb_checkers > 0 && gameState->zones[i].player == EPlayer1)
                sum += gameState->zones[i].nb_checkers;
        }

        sum += gameState->zones[EPos_OutP1].nb_checkers;
    }
    else
    {
        for (i=23; i>17; i--)
        {
            if (gameState->zones[i].nb_checkers > 0 && gameState->zones[i].player == EPlayer2)
                sum += gameState->zones[i].nb_checkers;
        }

        sum += gameState->zones[EPos_OutP2].nb_checkers;
    }

    return (sum == 15);
}

/* Fonction qui determine si le jeu est fini
 * @param S_GameState* gameState
 *     Etat du jeu
 * @return int
 *     1 si le jeu est fini, 0 sinon
 */
int IsFinish(S_GameState* gameState)
{
    EPosition pos = (gameState->currentPlayer == EPlayer1) ? EPos_OutP1 : EPos_OutP2;
    return (gameState->zones[pos].nb_checkers == 15);
}