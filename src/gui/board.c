#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "cste.h"
#include "library.h"
#include "menu.h"
#include "board.h"

/* Procedure d'initialisation de l'etat du jeu
 * @param S_GameState* gameState
 *     Etat du jeu
 * @param S_GameConfig gameConfig
 *     Structure de configuration du jeu
 */
void InitGameState(S_GameState* gameState, S_GameConfig gameConfig)
{
    gameState->die1 = 0;
    gameState->die2 = 0;

    gameState->scoreP1 = 0;
    gameState->scoreP2 = 0;

    gameState->gameConfig = gameConfig;

    // On n'initialise pas le proprietaire du videau puisqu'il manque un etat "aucun joueur"
    // -> Verifier si la mise est superieure a 1 pour regarder le proprietaire
    gameState->stake = 1;

    // On met a zero toutes les zones.
    // On n'initialise pas le joueur puisqu'il manque un etat "aucun joueur"
    // -> Toujours verifier que le nombre de pions est non nul avant de regarder le joueur
    int i;

    for (i=0; i<28; i++)
        gameState->zones[i].nb_checkers = 0;

    // Chaque joueur commence avec deux dames sur sa case 24,
    // trois sur sa case 8, et cinq sur ses cases 13 et 6.

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

/* Procedure pour afficher les pions
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

    clip.y = 40; clip.w = 40; clip.h = 40;

    // Dessin des pions sur les fleches
    for (i=0; i<24; i++)
    {
        if (gameState.zones[i].nb_checkers > 0)
        {
            clip.x = (gameState.zones[i].player == EPlayer1)? 0 : CHECKER_W;
            if (gameState.gameConfig.player1Color == WHITE)
                clip.x = CHECKER_W - clip.x;

            // Calcul de la position x des pions
            position.x = BORDER_W + (ZONE_W - CHECKER_W)/2;
            position.x += ZONE_W * ((i < 12)? 11-i : i-12); // decalage des fleches

            if (i > 17 || i < 6)
                position.x += 2*BORDER_W;

            double step = 40.0;

            if (gameState.zones[i].nb_checkers > 4)
                step = 120.0 / (gameState.zones[i].nb_checkers - 1);

            for (j=0; j<gameState.zones[i].nb_checkers; j++)
            {
                if (i < 12)
                    position.y = 450 - BORDER_W - j*step;
                else
                    position.y = BORDER_W + j*step;

                SDL_BlitSurface(overlays, &clip, window, &position);
            }
        }
    }

    // Dessin des pions prisonniers
    TTF_Font *font = TTF_OpenFont(DESIGN_PATH "numbers.ttf", 16);
    SDL_Color black = {0,0,0}, white = {255,255,255};
    char nb_txt[4] = {'\0'};
    SDL_Surface *nb = NULL;

    if (gameState.zones[EPos_BarP1].nb_checkers > 0)
    {
        clip.x = (gameState.gameConfig.player1Color == BLACK) ? 0 : 40;
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
            position.y = BORDER_W + i*12;
            SDL_BlitSurface(overlays, &clip, window, &position);
        }
    }

    if (gameState.zones[EPos_OutP2].nb_checkers > 0)
    {
        clip.y = 52;
        for (i=0; i<gameState.zones[EPos_OutP2].nb_checkers; i++)
        {
            position.y = 463 - i*12;
            SDL_BlitSurface(overlays, &clip, window, &position);
        }
    }

    // Dessin du videau
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


    TTF_CloseFont(font);
    SDL_FreeSurface(overlays);
}

/* Fonction de gestion de l'affichage du plateau de jeu
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param E_GameMode gameMode
 *     Mode de jeu du programme en cours
 * @param S_AIFunctions* aiFunctions
 *     Structure de stockage des fonctions des bibliotheques
 * @param S_GameConfig gameConfig
 *     Structure de configuration du jeu
 * @return int
 *     1 si on doit quitter le programme, 0 sinon
 */
int DisplayBoard(SDL_Surface* window, E_GameMode gameMode, S_AIFunctions* aiFunctions, S_GameConfig gameConfig)
{
    SDL_Surface *board_bg = IMG_Load(DESIGN_PATH "board.png");

    S_GameState gameState;
    InitGameState(&gameState, gameConfig);

    SDL_Rect position;
    position.x = 0; position.y = 0;

    SDL_BlitSurface(board_bg, NULL, window, &position);

    DisplayCheckers(window, gameState);
    //DisplayScore(window, gameConfig);

    int quit = 0, finish = 0;
    SDL_Event event;

    while(!finish)
    {
        // Affichage
        if (event.type == SDL_MOUSEBUTTONUP ||
            event.type == SDL_MOUSEBUTTONDOWN ||
            event.type == SDL_KEYUP)
        {
            SDL_BlitSurface(board_bg, NULL, window, &position);
        }

        SDL_Flip(window);

        // Gestion des evenements
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                finish = 1;
                quit = 1;
                break;
        }
    }

    SDL_FreeSurface(board_bg);

    return quit;
}
