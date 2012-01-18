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
            gameState->zones[zone].player == gameState->currentPlayer &&
            gameState->zones[gameState->currentPlayer == EPlayer1 ? EPos_OutP1 : EPos_OutP2].nb_checkers == 0);
}

/* Fonction qui indique si la zone cliquee est valide comm destination
 * @param int zone
 *     Zone cliquee
 * @param S_GameState* gameState
 *     Etat du jeu
 * @return int
 *     1 si la zone cliquee est valide, 0 sinon
 */
int IsValidDst(int zone, S_GameState* gameState)
{
    int valid = 0;
    if(gameState->useDie1 > 0)
    {
        valid |= (zone - gameState->die1 == gameState->currentZone &&
                  gameState->currentPlayer == EPlayer2) ||
                  (zone + gameState->die1 == gameState->currentZone &&
                  gameState->currentPlayer == EPlayer1);
    }

    if(gameState->useDie2 > 0)
    {
        valid |= (zone - gameState->die2 == gameState->currentZone &&
                  gameState->currentPlayer == EPlayer2) ||
                  (zone + gameState->die2 == gameState->currentZone &&
                  gameState->currentPlayer == EPlayer1);
    }

    valid &= (zone != -1);
    valid &= (zone != EPos_BarP1);
    valid &= (zone != EPos_BarP2);
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
    // gestion d'un blot
    if (gameState->zones[zone].nb_checkers == 1 && gameState->zones[zone].player != gameState->currentPlayer)
    {
        gameState->zones[zone].nb_checkers--;

        if (gameState->currentPlayer == EPlayer1)
            gameState->zones[EPos_BarP2].nb_checkers++;
        else
            gameState->zones[EPos_BarP1].nb_checkers++;
    }

    gameState->zones[gameState->currentZone].nb_checkers--;
    gameState->zones[zone].nb_checkers++;
    gameState->zones[zone].player = gameState->currentPlayer;

    int diff = abs(gameState->currentZone - zone);

    if (gameState->die1 == diff)
        gameState->useDie1--;

    if (gameState->die2 == diff)
        gameState->useDie2--;

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

            gameState->currentStage = WAITING_ROLL_DBL;
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
    return 1;
}

/* Fonction qui determine si le jeu est fini
 * @param S_GameState* gameState
 *     Etat du jeu
 * @return int
 *     1 si le jeu est fini, 0 sinon
 */
int IsFinish(S_GameState* gameState)
{
    return 0;
}