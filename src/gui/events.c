#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "cste.h"
#include "structures.h"
#include "gui.h"
#include "events.h"
#include "board.h"
#include "game.h"

/* Procedure de gestion des evenements du menu
 * @param SDL_Event* event
 *     Evenements de la fenetre
 * @param S_GameConfig gameConfig
 *     Structure de configuration du jeu
 * @param E_MenuSelected selected
 *     Selection du menu
 * @return E_MenuSelected
 *     Eventuel bouton clique
 */
E_MenuSelected EventsMenu(SDL_Event* event, S_GameConfig* gameConfig, E_MenuSelected* selected)
{
    E_MenuSelected clicked = NONE;

    SDL_WaitEvent(event);
    switch(event->type)
    {
        case SDL_QUIT:
            clicked = QUIT;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (ClickRect(event, 450, 446, 150, 45))
                    *selected = QUIT;

            if (ClickRect(event, 158, 446, 150, 45))
                    *selected = START;

            break;
        case SDL_MOUSEBUTTONUP:
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                // Boutons
                if (ClickRect(event, 450, 446, 150, 45))
                    clicked = QUIT;

                if (ClickRect(event, 158, 446, 150, 45))
                {
                    if (strlen(gameConfig->namePlayer1) > 0 && strlen(gameConfig->namePlayer2) > 0)
                        clicked = START;
                }

                // Selection des zones de texte
                if (ClickRect(event, 379, 126, 300, 40))
                    *selected = PLAYER1;
                else if (ClickRect(event, 379, 186, 300, 40))
                    *selected = PLAYER2;
                else
                    *selected = NONE;

                // Clic sur les boutons
                if (ClickRect(event, 379, 246, 40, 40))
                    gameConfig->player1Color = BLACK;

                if (ClickRect(event, 568, 246, 40, 40))
                    gameConfig->player1Color = WHITE;

                if (ClickRect(event, 41, 306, 40, 40))
                    gameConfig->option = 1;

                if (ClickRect(event, 420, 306, 40, 40))
                    gameConfig->option = 0;

                // Clic sur les fleches
                if (ClickRect(event, 444, 366, 30, 40))
                {
                    if (gameConfig->points > MIN_POINTS)
                        gameConfig->points -= 2;
                }

                if (ClickRect(event, 584, 366, 30, 40))
                {
                    if (gameConfig->points < MAX_POINTS)
                        gameConfig->points += 2;
                }
            }
            break;
        case SDL_KEYUP:
            if (*selected == PLAYER1)
                TextInput(gameConfig->namePlayer1, event->key.keysym);
            else if (*selected == PLAYER2)
                TextInput(gameConfig->namePlayer2, event->key.keysym);

            break;
    }

    return clicked;
}

/* Fonction de gestion des evenements du plateau
 * @param SDL_Event* event
 *     Evenements de la fenetre
 * @param S_GameState* gameState
 *     Etat du jeu
 * @return E_BoardSelected
 *     Eventuel bouton clique
 */
E_BoardSelected EventsBoard(SDL_Event* event, S_GameState* gameState)
{
    E_BoardSelected clicked = NONE_BOARD;

    if (gameState->gameConfig.mode == HUMAN_HUMAN)
        SDL_WaitEvent(event); // On attend l'evenement
    else
        SDL_PollEvent(event); // Defilement automatique

    int zone = -1;
    int bx;

    if (event->type == SDL_QUIT)
        clicked = QUIT_BOARD;

    switch (gameState->currentStage)
    {
        case WAITING_FIRST_ROLL:
            switch(event->type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    // Bouton "Lancer"
                    if (ClickRect(event, 293, 230, 100, 30))
                        gameState->selected = BUTTON1;
                    break;
                case SDL_MOUSEBUTTONUP:
                    gameState->selected = NONE_BOARD;

                    // Bouton "Lancer"
                    if (ClickRect(event, 293, 230, 100, 30))
                        RollDice(gameState);
                    break;
            }
            break;
        case WAITING_ROLL_DBL:
            // On gere les boutons que si le joueur est humain
            if (IsHuman(gameState, 1))
            {
                bx = (gameState->currentPlayer == EPlayer1) ? 122 : 464;

                switch(event->type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        // Bouton "Doubler"
                        if (ClickRect(event, bx, 210, 100, 30))
                            gameState->selected = BUTTON1;

                        // Bouton "Lancer"
                        if (ClickRect(event, bx, 250, 100, 30))
                            gameState->selected = BUTTON2;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        gameState->selected = NONE_BOARD;

                        // Bouton "Doubler"
                        if (ClickRect(event, bx, 210, 100, 30))
                            gameState->currentStage = DOUBLE_POPUP;

                        // Bouton "Lancer"
                        if (ClickRect(event, bx, 250, 100, 30))
                        {
                            RollDice(gameState);

                            if (IsPossibleMove(gameState))
                                gameState->currentStage = SELECT_ZONE_SRC;
                            else
                                gameState->currentStage = PASS_POPUP;
                        }
                        break;
                }
            }
            break;
        case WAITING_ROLL:
            // On gere le bouton que si le joueur est humain
            if (IsHuman(gameState, 1))
            {
                bx = (gameState->currentPlayer == EPlayer1) ? 122 : 464;

                switch(event->type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        // Bouton "Lancer"
                        if (ClickRect(event, bx, 230, 100, 30))
                            gameState->selected = BUTTON1;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        gameState->selected = NONE_BOARD;

                        // Bouton "Lancer"
                        if (ClickRect(event, bx, 230, 100, 30))
                        {
                            RollDice(gameState);

                            if (IsPossibleMove(gameState))
                                gameState->currentStage = SELECT_ZONE_SRC;
                            else
                                gameState->currentStage = PASS_POPUP;
                        }
                        break;
                }
            }
            break;
        case DOUBLE_POPUP:
            // On gere la popup que si l'adversaire est humain
            if (IsHuman(gameState, 0))
            {
                switch(event->type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        // Bouton "Accepter"
                        if (ClickRect(event, 234, 305, 100, 30))
                            gameState->selected = BUTTON1;

                        // Bouton "Refuser"
                        if (ClickRect(event, 354, 305, 100, 30))
                            gameState->selected = BUTTON2;
                        break;

                    case SDL_MOUSEBUTTONUP:
                        gameState->selected = NONE_BOARD;

                        // Bouton "Accepter"
                        if (ClickRect(event, 234, 305, 100, 30))
                        {
                            gameState->stake *= 2;

                            if (gameState->currentPlayer == EPlayer1)
                                gameState->cubeOwner = EPlayer2;
                            else
                                gameState->cubeOwner = EPlayer1;

                            gameState->currentStage = WAITING_ROLL;
                        }

                        // Bouton "Refuser"
                        if (ClickRect(event, 354, 305, 100, 30))
                        {
                            // Le joueur courant gagne la partie
                            if (gameState->currentPlayer == EPlayer1)
                                gameState->scoreP1 += gameState->stake;
                            else
                                gameState->scoreP2 += gameState->stake;

                            gameState->currentStage = FINISH_GAME_POPUP;
                        }
                        break;
                }
            }
            break;
        case FIRST_ROLL_POPUP:
            // On gere la popup que si un humain joue
            if (gameState->gameConfig.mode != AI_AI)
            {
                switch(event->type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        // Bouton "OK"
                        if (ClickRect(event, 293, 305, 100, 30))
                            gameState->selected = BUTTON1;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        gameState->selected = NONE_BOARD;

                        // Bouton "OK"
                        if (ClickRect(event, 293, 305, 100, 30))
                        {
                            if (IsPossibleMove(gameState))
                                gameState->currentStage = SELECT_ZONE_SRC;
                            else
                                gameState->currentStage = PASS_POPUP;
                        }
                        break;
                }
            }
            break;
        case PASS_POPUP:
            // On gere la popup que si le joueur courant est humain
            if (IsHuman(gameState, 1))
            {
                switch(event->type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        // Bouton "OK"
                        if (ClickRect(event, 293, 305, 100, 30))
                            gameState->selected = BUTTON1;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        gameState->selected = NONE_BOARD;

                        // Bouton "OK"
                        if (ClickRect(event, 293, 305, 100, 30))
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
                        break;
                }
            }
            break;
        case FINISH_GAME_POPUP:
            // On gere la popup que si un joueur est humain
            if (gameState->gameConfig.mode != AI_AI)
            {
                switch(event->type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        // Bouton "OK"
                        if (ClickRect(event, 293, 305, 100, 30))
                            gameState->selected = BUTTON1;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        gameState->selected = NONE_BOARD;

                        // Bouton "OK"
                        if (ClickRect(event, 293, 305, 100, 30))
                        {
                            // On desalloue les IA
                            if (gameState->gameConfig.mode == HUMAN_AI)
                                gameState->gameConfig.aiFunctions[1].AI_EndGame();

                            // Fin du match
                            if ((gameState->currentPlayer == EPlayer1 && gameState->scoreP1 >= gameState->gameConfig.points) ||
                                (gameState->currentPlayer == EPlayer2 && gameState->scoreP2 >= gameState->gameConfig.points))
                                gameState->currentStage = FINISH_MATCH_POPUP;
                            else
                            {
                                // On reinitialise les IA pour une nouvelle manche
                                if (gameState->gameConfig.mode == HUMAN_AI)
                                    gameState->gameConfig.aiFunctions[1].AI_StartGame();

                                // Remise a zero du plateau
                                InitGameState(gameState, gameState->gameConfig);
                            }
                        }
                        break;
                }
            }
            break;
        case FINISH_MATCH_POPUP:
            switch(event->type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    // Bouton "OK"
                    if (ClickRect(event, 293, 305, 100, 30))
                        gameState->selected = BUTTON1;
                    break;
                case SDL_MOUSEBUTTONUP:
                    gameState->selected = NONE_BOARD;

                    // Bouton "OK"
                    if (ClickRect(event, 293, 305, 100, 30))
                    {
                        // On ecrit dans le fichier des scores
                        FILE* file = fopen("score.txt", "a");

                        fprintf(file, "%s : %d\n", gameState->gameConfig.namePlayer1, gameState->scoreP1);
                        fprintf(file, "%s : %d\n", gameState->gameConfig.namePlayer2, gameState->scoreP2);
                        fprintf(file, "--------------------\n");

                        fclose(file);

                        // On revient au menu
                        clicked = MENU_BOARD;
                    }
                    break;
            }
            break;

        case SELECT_ZONE_SRC:
            // On gere le deplacement que si le joueur courant est humain
            if (IsHuman(gameState, 1))
            {
                switch(event->type)
                {
                    case SDL_MOUSEBUTTONUP:
                        // Selection d'une zone source
                        zone = ClickZone(event);
                        if (IsValidSrc(zone, gameState))
                        {
                            gameState->currentZone = zone;
                            gameState->currentStage = SELECT_ZONE_DST;
                        }
                        break;
                }
            }
            break;
        case SELECT_ZONE_DST:
            // On gere le deplacement que si le joueur courant est humain
            if (IsHuman(gameState, 1))
            {
                switch(event->type)
                {
                    case SDL_MOUSEBUTTONUP:
                        // Selection d'une zone source
                        zone = ClickZone(event);

                        if (IsValidDst(zone, gameState))
                            DoMove(zone, gameState);
                        else if (IsValidSrc(zone, gameState))
                            gameState->currentZone = zone;
                        else
                        {
                            gameState->currentZone = -1;
                            gameState->currentStage = SELECT_ZONE_SRC;
                        }
                        break;
                }
            }
            break;
        default:
            break;
    }

    return clicked;
}

/* Procedure de gestion des actions des IA
 * @param S_GameState* gameState
 *     Etat du jeu
 */
void AI_EventsBoard(S_GameState* gameState)
{
    switch (gameState->currentStage)
    {
        case WAITING_ROLL_DBL:
            if (!IsHuman(gameState, 1))
            {
                // On demande a l'IA si elle veut doubler ou pas
                int answer = 0;
                SGameState AI_gameState;
                CreateAIGameState(&AI_gameState, gameState);

                if (gameState->currentPlayer == EPlayer1)
                    answer = gameState->gameConfig.aiFunctions[0].AI_DoubleStack(&AI_gameState);
                else
                    answer = gameState->gameConfig.aiFunctions[1].AI_DoubleStack(&AI_gameState);

                // L'IA double
                if (answer)
                    gameState->currentStage = DOUBLE_POPUP;
                else
                {
                    RollDice(gameState);

                    if (IsPossibleMove(gameState))
                        gameState->currentStage = SELECT_ZONE_SRC;
                    else
                        gameState->currentStage = PASS_POPUP;
                }
            }
            break;
        case WAITING_ROLL:
            // On gere le bouton que si le joueur est humain
            if (!IsHuman(gameState, 1))
            {
                RollDice(gameState);

                if (IsPossibleMove(gameState))
                    gameState->currentStage = SELECT_ZONE_SRC;
                else
                    gameState->currentStage = PASS_POPUP;
            }
            break;
        case DOUBLE_POPUP:
            if (!IsHuman(gameState, 0))
            {
                // On demande a l'IA si elle accepte ou refuse le double
                int answer = 0;
                SGameState AI_gameState;
                CreateAIGameState(&AI_gameState, gameState);

                if (gameState->currentPlayer == EPlayer1)
                    answer = gameState->gameConfig.aiFunctions[1].AI_TakeDouble(&AI_gameState);
                else
                    answer = gameState->gameConfig.aiFunctions[0].AI_TakeDouble(&AI_gameState);

                // l'IA accepte le double
                if (answer)
                {
                    gameState->stake *= 2;

                    if (gameState->currentPlayer == EPlayer1)
                        gameState->cubeOwner = EPlayer2;
                    else
                        gameState->cubeOwner = EPlayer1;

                    gameState->currentStage = WAITING_ROLL;
                }

                // l'IA refuse le double
                else
                {
                    // Le joueur courant gagne la partie
                    if (gameState->currentPlayer == EPlayer1)
                        gameState->scoreP1 += gameState->stake;
                    else
                        gameState->scoreP2 += gameState->stake;

                    gameState->currentStage = FINISH_GAME_POPUP;
                }
            }
            break;
        case FIRST_ROLL_POPUP:
            // On passe la popup
            if (gameState->gameConfig.mode == AI_AI)
            {
                if (IsPossibleMove(gameState))
                    gameState->currentStage = SELECT_ZONE_SRC;
                else
                    gameState->currentStage = PASS_POPUP;
            }
            break;
        case PASS_POPUP:
            // On passe la popup
            if (!IsHuman(gameState, 1))
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
            break;
        case FINISH_GAME_POPUP:
            // On passe la popup
            if (gameState->gameConfig.mode == AI_AI)
            {
                // On desalloue les IA
                gameState->gameConfig.aiFunctions[0].AI_EndGame();
                gameState->gameConfig.aiFunctions[1].AI_EndGame();

                if ((gameState->currentPlayer == EPlayer1 && gameState->scoreP1 >= gameState->gameConfig.points) ||
                    (gameState->currentPlayer == EPlayer2 && gameState->scoreP2 >= gameState->gameConfig.points))
                    gameState->currentStage = FINISH_MATCH_POPUP;
                else
                {
                    // On reinitialise les IA
                    gameState->gameConfig.aiFunctions[0].AI_StartGame();
                    gameState->gameConfig.aiFunctions[1].AI_StartGame();

                    // Remise a zero du plateau
                    InitGameState(gameState, gameState->gameConfig);
                }
            }
            break;
        case SELECT_ZONE_SRC:
            if (!IsHuman(gameState, 1))
            {
                int done = 0;
                int player = (gameState->currentPlayer == EPlayer1) ? 0 : 1;
                int error = 0;

                while (gameState->aiErrors[player] <= 3 && !done)
                {
                    SGameState AI_gameState;
                    CreateAIGameState(&AI_gameState, gameState);

                    SMove moves[4];
                    int i;

                    for (i=0; i<4; i++)
                    {
                        moves[i].src_point = EPos_nopos;
                        moves[i].dest_point = EPos_nopos;
                    }

                    if (gameState->currentPlayer == EPlayer1)
                        gameState->gameConfig.aiFunctions[0].AI_MakeDecision(&AI_gameState, moves, error != 0);
                    else
                        gameState->gameConfig.aiFunctions[1].AI_MakeDecision(&AI_gameState, moves, error != 0);

                    if (IsValidAIMoves(*gameState, moves))
                    {
                        // On effectue les mouvements
                        for (i=0; i<4; i++)
                        {
                            if (IsPossibleMove(gameState))
                            {
                                if (moves[i].src_point != EPos_nopos && moves[i].dest_point != EPos_nopos)
                                {
                                    int src = ConvertAIZone(gameState, moves[i].src_point);
                                    int dest = ConvertAIZone(gameState, moves[i].dest_point);

                                    gameState->currentZone = src;
                                    DoMove(dest, gameState);
                                    done = 1;
                                    gameState->refresh = 1;
                                }
                            }
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Mouvement incorrect de l'IA (joueur %i)\n", gameState->currentPlayer + 1);
                        gameState->aiErrors[player]++;
                        error++;
                        gameState->refresh = 1;

                        if (gameState->aiErrors[player] > 3)
                        {
                            // Le joueur courant gagne la partie
                            if (gameState->currentPlayer == EPlayer1)
                                gameState->scoreP2 += gameState->stake;
                            else
                                gameState->scoreP1 += gameState->stake;

                            gameState->currentStage = FINISH_GAME_POPUP;
                        }
                    }
                }
            }
            break;
        default:
            break;
    }
}