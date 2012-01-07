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
 */
void InitGameState(S_GameState* gameState)
{
    gameState->die1 = 0;
    gameState->die2 = 0;

    gameState->scoreP1 = 0;
    gameState->scoreP2 = 0;

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
    InitGameState(&gameState);

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
