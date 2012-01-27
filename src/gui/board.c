#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "cste.h"
#include "structures.h"
#include "gui.h"
#include "board.h"
#include "game.h"
#include "events.h"

/* Procedure d'initialisation de l'etat du jeu
 * @param S_GameState* gameState
 *     Etat du jeu
 * @param S_GameConfig gameConfig
 *     Structure de configuration du jeu
 */
void InitGameState(S_GameState* gameState, S_GameConfig gameConfig)
{
    int i;

    gameState->die1 = 0; gameState->useDie1 = 0;
    gameState->die2 = 0; gameState->useDie2 = 0;

    gameState->gameConfig = gameConfig;

    gameState->currentStage = WAITING_FIRST_ROLL;
    gameState->currentZone = -1;

    gameState->selected = NONE_BOARD;
    gameState->refresh = 0;
    gameState->aiErrors[0] = 0;
    gameState->aiErrors[1] = 0;

    // On n'initialise pas le proprietaire du videau puisqu'il manque un etat "aucun joueur"
    // -> Verifier si la mise est superieure a 1 pour regarder le proprietaire
    gameState->stake = 1;

    // On met a zero toutes les zones.
    // On n'initialise pas le joueur puisqu'il manque un etat "aucun joueur"
    // Sauf pour les sorties et la barre
    // -> Toujours verifier que le nombre de pions est non nul avant de regarder le joueur
    for (i=0; i<28; i++)
        gameState->zones[i].nb_checkers = 0;

    gameState->zones[EPos_BarP1].player = EPlayer1;
    gameState->zones[EPos_BarP2].player = EPlayer2;
    gameState->zones[EPos_OutP1].player = EPlayer1;
    gameState->zones[EPos_OutP2].player = EPlayer2;

    // On positionne les pions de depart
    gameState->zones[EPos_24].player = EPlayer1;
    gameState->zones[EPos_24].nb_checkers = 2;

    gameState->zones[EPos_13].player = EPlayer1;
    gameState->zones[EPos_13].nb_checkers = 5;

    gameState->zones[EPos_8].player = EPlayer1;
    gameState->zones[EPos_8].nb_checkers = 3;

    gameState->zones[EPos_6].player = EPlayer1;
    gameState->zones[EPos_6].nb_checkers = 5;

    gameState->zones[EPos_1].player = EPlayer2;
    gameState->zones[EPos_1].nb_checkers = 2;

    gameState->zones[EPos_12].player = EPlayer2;
    gameState->zones[EPos_12].nb_checkers = 5;

    gameState->zones[EPos_17].player = EPlayer2;
    gameState->zones[EPos_17].nb_checkers = 3;

    gameState->zones[EPos_19].player = EPlayer2;
    gameState->zones[EPos_19].nb_checkers = 5;
}

/* Procedure pour afficher les dames
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param S_GameState gameState
 *     Etat du jeu
 */
void DisplayCheckers(SDL_Surface* window, S_GameState gameState)
{
    int i, j;
    SDL_Surface *overlays = IMG_Load(DESIGN_PATH "overlays.png");
    SDL_Rect clip, position;

    clip.w = 40; clip.h = 40;

    // Dessin des pions sur les fleches
    for (i=0; i<24; i++)
    {
        if (gameState.zones[i].nb_checkers > 0)
        {
            clip.y = 40;
            clip.x = (gameState.zones[i].player == EPlayer1)? 0 : CHECKER_W;
            if (gameState.gameConfig.player1Color == WHITE)
                clip.x = CHECKER_W - clip.x;

            // Calcul de l'abscisse des dames
            position.x = BORDER + (ZONE_W - CHECKER_W)/2;
            position.x += ZONE_W * ((i < 12)? 11-i : i-12); // decalage des fleches

            if (i > 17 || i < 6)
                position.x += 2 * BORDER;

            // Gestion de l'empilement des dames
            double step = 40.0;

            if (gameState.zones[i].nb_checkers > 4)
                step = 120.0 / (gameState.zones[i].nb_checkers - 1);

            for (j=0; j<gameState.zones[i].nb_checkers; j++)
            {
                if (i < 12)
                    position.y = 450 - BORDER - j*step;
                else
                    position.y = BORDER + j*step;

                SDL_BlitSurface(overlays, &clip, window, &position);
            }

            // Gestion de la selection des pions
            if (gameState.currentZone == i)
            {
                clip.y = 80;
                SDL_BlitSurface(overlays, &clip, window, &position);
            }
        }
    }

    // Dessin des pions prisonniers
    TTF_Font *font = TTF_OpenFont(DESIGN_PATH "board.ttf", 16);
    SDL_Color black = {0,0,0}, white = {255,255,255};
    char nb_txt[4] = {'\0'};
    SDL_Surface *nb = NULL;

    if (gameState.zones[EPos_BarP1].nb_checkers > 0)
    {
        clip.x = (gameState.gameConfig.player1Color == BLACK) ? 0 : 40;
        clip.y = (gameState.currentZone == EPos_BarP1) ? 80 : 40;
        position.x = 322; position.y = 185;
        SDL_BlitSurface(overlays, &clip, window, &position);

        // Dessin du nombre de prisonniers
        if (gameState.zones[EPos_BarP1].nb_checkers > 1)
        {
            SDL_Color color = (gameState.gameConfig.player1Color == BLACK)? white : black;
            sprintf(nb_txt, "%i", gameState.zones[EPos_BarP1].nb_checkers); nb_txt[2] = '\0';
            nb = TTF_RenderText_Blended(font, nb_txt, color);
            position.x = 342 - nb->w/2; position.y = 205 - nb->h/2;
            SDL_BlitSurface(nb, NULL, window, &position);
            SDL_FreeSurface(nb);
        }
    }

    if (gameState.zones[EPos_BarP2].nb_checkers > 0)
    {
        clip.x = (gameState.gameConfig.player1Color == BLACK) ? 40 : 0;
        clip.y = (gameState.currentZone == EPos_BarP2) ? 80 : 40;
        position.x = 322; position.y = 265;
        SDL_BlitSurface(overlays, &clip, window, &position);

        // Dessin du nombre de prisonniers
        if (gameState.zones[EPos_BarP2].nb_checkers > 1)
        {
            SDL_Color color = (gameState.gameConfig.player1Color == BLACK)? black : white;
            sprintf(nb_txt, "%i", gameState.zones[EPos_BarP2].nb_checkers); nb_txt[2] = '\0';
            nb = TTF_RenderText_Blended(font, nb_txt, color);
            position.x = 342 - nb->w/2; position.y = 285 - nb->h/2;
            SDL_BlitSurface(nb, NULL, window, &position);
            SDL_FreeSurface(nb);
        }
    }

    // Dessin des pions sortis
    clip.w = 46; clip.h = 12; clip.x = 80; position.x = 697;

    if (gameState.zones[EPos_OutP1].nb_checkers > 0)
    {
        clip.y = 40;
        for (i=0; i<gameState.zones[EPos_OutP1].nb_checkers; i++)
        {
            position.y = 463 - i*12;
            SDL_BlitSurface(overlays, &clip, window, &position);
        }
    }

    if (gameState.zones[EPos_OutP2].nb_checkers > 0)
    {
        clip.y = 52;
        for (i=0; i<gameState.zones[EPos_OutP2].nb_checkers; i++)
        {
            position.y = BORDER + i*12;
            SDL_BlitSurface(overlays, &clip, window, &position);
        }
    }

    // Dessin du videau
    if (gameState.currentStage != WAITING_FIRST_ROLL)
    {
        clip.x = 366; clip.y = 40; position.x = 324; clip.w = 36; clip.h = 36;

        if (gameState.stake == 1)
            position.y = 227;
        else if (gameState.cubeOwner == EPlayer1)
            position.y = 90;
        else
            position.y = 364;

        SDL_BlitSurface(overlays, &clip, window, &position);

        sprintf(nb_txt, "%i", gameState.stake); nb_txt[3] = '\0';
        nb = TTF_RenderText_Blended(font, nb_txt, black);
        position.x = 342 - nb->w/2; position.y += 18 - nb->h/2;
        SDL_BlitSurface(nb, NULL, window, &position);
        SDL_FreeSurface(nb);
    }

    TTF_CloseFont(font);
    SDL_FreeSurface(overlays);
}

/* Procedure pour afficher les boutons et popup
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param S_GameState gameState
 *     Etat du jeu
 */
void DisplayBoardOverlays(SDL_Surface* window, S_GameState gameState)
{
    switch (gameState.currentStage)
    {
        case WAITING_FIRST_ROLL:
            DisplayButton(window, CENTER_X, CENTER_Y, "Lancer", gameState.selected == BUTTON1);
            break;
        case WAITING_ROLL_DBL:
            // On affiche les boutons que si le joueur est humain
            if (IsHuman(&gameState, 1))
            {
                DisplayButton(window, (gameState.currentPlayer == EPlayer1) ? CENTER_LEFT : CENTER_RIGHT,
                                        CENTER_Y - 20, "Doubler", gameState.selected == BUTTON1);
                DisplayButton(window, (gameState.currentPlayer == EPlayer1) ? CENTER_LEFT : CENTER_RIGHT,
                                        CENTER_Y + 20, "Lancer", gameState.selected == BUTTON2);
            }
            break;
        case WAITING_ROLL:
            // On affiche les boutons que si le joueur est humain
            if (IsHuman(&gameState, 1))
            {
                DisplayButton(window, (gameState.currentPlayer == EPlayer1) ? CENTER_LEFT : CENTER_RIGHT,
                                        CENTER_Y, "Lancer", gameState.selected == BUTTON1);
            }
            break;
        case FIRST_ROLL_POPUP:
            DisplayDice(window, gameState);

            // On affiche la popup que si un humain joue
            if (gameState.gameConfig.mode != AI_AI)
            {
                if (gameState.currentPlayer == EPlayer1)
                    DisplayPopup(window, 3, "Le joueur", gameState.gameConfig.namePlayer1, "commence.");
                else
                    DisplayPopup(window, 3, "Le joueur", gameState.gameConfig.namePlayer2, "commence.");

                DisplayButton(window, CENTER_X, 320, "OK", gameState.selected == BUTTON1);
            }
            break;
        case SELECT_ZONE_DST:
            DisplayNumbers(window, gameState);
            DisplayDice(window, gameState);

            if (gameState.gameConfig.option && gameState.gameConfig.mode != AI_AI)
                DisplayHelp(window, gameState);
            break;
        case PASS_POPUP:
            DisplayDice(window, gameState);

            // On affiche la popup que si le joueur est humain
            if (IsHuman(&gameState, 1))
            {
                if (gameState.currentPlayer == EPlayer1)
                    DisplayPopup(window, 3, "Le joueur", gameState.gameConfig.namePlayer1, "passe son tour.");
                else
                    DisplayPopup(window, 3, "Le joueur", gameState.gameConfig.namePlayer2, "passe son tour.");

                DisplayButton(window, CENTER_X, 320, "OK", gameState.selected == BUTTON1);
            }
            break;
        case DOUBLE_POPUP:
            // On affiche la popup que si le joueur adverse est humain
            if (IsHuman(&gameState, 0))
            {
                if (gameState.currentPlayer == EPlayer1)
                    DisplayPopup(window, 3, "Le joueur", gameState.gameConfig.namePlayer1, "veut doubler.");
                else
                    DisplayPopup(window, 3, "Le joueur", gameState.gameConfig.namePlayer2, "veut doubler.");

                DisplayButton(window, CENTER_X - 60, 320, "Accepter", gameState.selected == BUTTON1);
                DisplayButton(window, CENTER_X + 60, 320, "Refuser", gameState.selected == BUTTON2);
            }
            break;
        case FINISH_GAME_POPUP:
            DisplayDice(window, gameState);

            // On affiche la popup que si un joueur est humain
            if (gameState.gameConfig.mode != AI_AI)
            {
                if (gameState.currentPlayer == EPlayer1)
                    DisplayPopup(window, 3, "Le joueur", gameState.gameConfig.namePlayer1, "a gagné la manche.");
                else
                    DisplayPopup(window, 3, "Le joueur", gameState.gameConfig.namePlayer2, "a gagné la manche.");

                DisplayButton(window, CENTER_X, 320, "OK", gameState.selected == BUTTON1);
            }
            break;
        case FINISH_MATCH_POPUP:
            DisplayDice(window, gameState);

            if (gameState.currentPlayer == EPlayer1)
                DisplayPopup(window, 3, "Le joueur", gameState.gameConfig.namePlayer1, "a gagné le match.");
            else
                DisplayPopup(window, 3, "Le joueur", gameState.gameConfig.namePlayer2, "a gagné le match.");

            DisplayButton(window, CENTER_X, 320, "Menu", gameState.selected == BUTTON1);

            break;
        default:
            DisplayNumbers(window, gameState);
            DisplayDice(window, gameState);
            break;
    }
}

/* Procedure pour afficher les des
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param S_GameState gameState
 *     Etat du jeu
 */
void DisplayDice(SDL_Surface* window, S_GameState gameState)
{
    SDL_Surface *overlays = IMG_Load(DESIGN_PATH "overlays.png");
    SDL_Rect clip, positionDie1, positionDie2;
    clip.y = 40; clip.w = 40; clip.h = 40;
    clip.x = 126 + (gameState.die1 - 1) * clip.w;

    positionDie1.y = CENTER_Y - clip.h/2;
    positionDie2.y = CENTER_Y - clip.h/2;

    // Affichage de chaque cote du plateau
    if (gameState.currentStage == FIRST_ROLL_POPUP)
    {
        positionDie1.x = CENTER_LEFT - clip.w/2;
        positionDie2.x = CENTER_RIGHT - clip.w/2;
    }

    // Affichage d'un seul cote
    else
    {
        if (gameState.currentPlayer == EPlayer1)
        {
            positionDie1.x = CENTER_LEFT - 10 - clip.w;
            positionDie2.x = CENTER_LEFT + 10;
        }
        else
        {
            positionDie1.x = CENTER_RIGHT - 10 - clip.w;
            positionDie2.x = CENTER_RIGHT + 10;
        }
    }

    if (gameState.useDie1 > 0)
        SDL_BlitSurface(overlays, &clip, window, &positionDie1);

    // Gestion des doubles des
    if (gameState.die1 == gameState.die2)
    {
        positionDie1.x -= (20 + clip.w);
        if (gameState.useDie1 > 1)
            SDL_BlitSurface(overlays, &clip, window, &positionDie1);
    }

    clip.x = 126 + (gameState.die2 - 1) * clip.w;
    if (gameState.useDie2 > 0)
        SDL_BlitSurface(overlays, &clip, window, &positionDie2);

    // Gestion des doubles des
    if (gameState.die1 == gameState.die2)
    {
        positionDie2.x += 20 + clip.w;
        if (gameState.useDie2 > 1)
            SDL_BlitSurface(overlays, &clip, window, &positionDie2);
    }

    SDL_FreeSurface(overlays);
}

/* Procedure pour afficher les numeros des fleches
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param S_GameState gameState
 *     Etat du jeu
 */
void DisplayNumbers(SDL_Surface* window, S_GameState gameState)
{
    SDL_Surface *numbers = IMG_Load(DESIGN_PATH "numbers.png");
    SDL_Rect clip, position;
    clip.x = 0; clip.w = 654; clip.h = 20;
    position.x = BORDER; position.y = 185;

    if (gameState.currentPlayer == EPlayer1)
    {
        clip.y = 20;
        SDL_BlitSurface(numbers, &clip, window, &position);
        clip.y = 0; position.y = 285;
        SDL_BlitSurface(numbers, &clip, window, &position);
    }
    else
    {
        clip.y = 0;
        SDL_BlitSurface(numbers, &clip, window, &position);
        clip.y = 20; position.y = 285;
        SDL_BlitSurface(numbers, &clip, window, &position);
    }

    SDL_FreeSurface(numbers);
}

/* Procedure pour afficher les aides au mouvement
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param S_GameState gameState
 *     Etat du jeu
 */
void DisplayHelp(SDL_Surface* window, S_GameState gameState)
{
    int i;
    SDL_Surface *overlays = IMG_Load(DESIGN_PATH "overlays.png");
    SDL_Rect clip, position;
    clip.x = 400; clip.h = 15; clip.w = 30;

    // Indicateur pour les fleches
    for (i=0; i<24; i++)
    {
        if (gameState.currentZone != -1 && IsValidDst(i, &gameState))
        {
            position.y = (i < 12) ? 270 : 205;
            clip.y = (i < 12) ? 15 : 0;

            position.x = BORDER + ZONE_W/2 -15;
            position.x += ZONE_W * ((i < 12) ? 11-i : i-12);

            if (i < 6 || i > 17)
                position.x += 2 * BORDER;

            SDL_BlitSurface(overlays, &clip, window, &position);
        }
    }

    // Indicateur pour la sortie
    position.x = 705;

    if (gameState.currentPlayer == EPlayer1)
    {
        if (gameState.currentZone != -1 && IsValidDst(EPos_OutP1, &gameState))
        {
            position.y = 270; clip.y = 15;
            SDL_BlitSurface(overlays, &clip, window, &position);
        }
    }
    else
    {
        if (gameState.currentZone != -1 && IsValidDst(EPos_OutP2, &gameState))
        {
            position.y = 205; clip.y = 0;
            SDL_BlitSurface(overlays, &clip, window, &position);
        }
    }

    SDL_FreeSurface(overlays);
}

/* Procedure pour afficher le score
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param S_GameState gameState
 *     Etat du jeu
 */
void DisplayScore(SDL_Surface* window, S_GameState gameState)
{
    TTF_Font *font = TTF_OpenFont(DESIGN_PATH "font.ttf", 23);
    SDL_Color black = {0, 0, 0};
    SDL_Rect position;

    char scorePlayer1[20] = {'\0'};
    char scorePlayer2[20] = {'\0'};

    sprintf(scorePlayer1, "%s : %i", gameState.gameConfig.namePlayer1, gameState.scoreP1);
    sprintf(scorePlayer2, "%s : %i", gameState.gameConfig.namePlayer2, gameState.scoreP2);

    position.y = BOTTOM - BORDER / 2;

    SDL_Surface *txtPlayer1 = TTF_RenderText_Blended(font, scorePlayer1, black);
    SDL_Surface *txtPlayer2 = TTF_RenderText_Blended(font, scorePlayer2, black);

    position.x = CENTER_LEFT - txtPlayer1->w/2;
    SDL_BlitSurface(txtPlayer1, NULL, window, &position);

    position.x = CENTER_RIGHT - txtPlayer2->w/2;
    SDL_BlitSurface(txtPlayer2, NULL, window, &position);

    SDL_FreeSurface(txtPlayer1);
    SDL_FreeSurface(txtPlayer2);
    TTF_CloseFont(font);
}

/* Fonction de gestion de l'affichage du plateau de jeu
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param S_GameConfig gameConfig
 *     Structure de configuration du jeu
 * @return int
 *     1 si on doit quitter le programme, 0 sinon
 */
int DisplayBoard(SDL_Surface* window, S_GameConfig gameConfig)
{
    SDL_Surface *board_bg = IMG_Load(DESIGN_PATH "board.png");

    S_GameState gameState;
    InitGameState(&gameState, gameConfig);

    // Initialisation du score
    gameState.scoreP1 = 0;
    gameState.scoreP2 = 0;

    SDL_Rect position;
    position.x = 0; position.y = 0;

    SDL_BlitSurface(board_bg, NULL, window, &position);
    DisplayCheckers(window, gameState);
    DisplayBoardOverlays(window, gameState);
    DisplayScore(window, gameState);

    // On initialise les IA
    if (gameConfig.mode != HUMAN_HUMAN)
    {
        gameConfig.aiFunctions[1].AI_StartMatch((unsigned int)(gameConfig.points));
        gameConfig.aiFunctions[1].AI_StartGame();
    }

    if (gameConfig.mode == AI_AI)
    {
        gameConfig.aiFunctions[0].AI_StartMatch((unsigned int)(gameConfig.points));
        gameConfig.aiFunctions[0].AI_StartGame();
    }

    // finish = On revient au menu
    // quit = On quitte le programme
    int quit = 0, finish = 0;
    SDL_Event event;

    int oldTime = SDL_GetTicks();
    int time;

    while(!finish)
    {
        // Affichage
        if (event.type == SDL_MOUSEBUTTONUP ||
            event.type == SDL_MOUSEBUTTONDOWN ||
            gameState.refresh)
        {
            SDL_BlitSurface(board_bg, NULL, window, &position);
            DisplayCheckers(window, gameState);
            DisplayBoardOverlays(window, gameState);
            DisplayScore(window, gameState);
            gameState.refresh = 0;
        }

        SDL_Flip(window);

        // Gestion des evenements
        E_BoardSelected button = EventsBoard(&event, &gameState);

        if (button == QUIT_BOARD)
        {
            finish = 1;
            quit = 1;

            // On desalloue les IA
            if (gameConfig.mode != HUMAN_HUMAN)
                gameConfig.aiFunctions[1].AI_EndGame();

            if (gameConfig.mode == AI_AI)
                gameConfig.aiFunctions[0].AI_EndGame();
        }
        else if (button == MENU_BOARD)
            finish = 1;

        // Gestion de la vitesse de jeu entre IA
        if (gameConfig.mode == AI_AI && gameState.currentStage == SELECT_ZONE_SRC && gameConfig.option)
        {
            time = SDL_GetTicks();

            if ((time - oldTime) > 1000)
            {
                AI_EventsBoard(&gameState);
                oldTime = time;
            }
        }
        else
            AI_EventsBoard(&gameState);

        SDL_Delay(5);
    }

    // On desalloue les IA
    if (gameConfig.mode != HUMAN_HUMAN)
        gameConfig.aiFunctions[1].AI_EndMatch();

    if (gameConfig.mode == AI_AI)
        gameConfig.aiFunctions[0].AI_EndMatch();

    SDL_FreeSurface(board_bg);

    return quit;
}
