#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "cste.h"
#include "structures.h"
#include "gui.h"
#include "menu.h"
#include "board.h"

/* Procedure de saisie du nom d'un joueur
 * @param char** name
 *      Nom du joueur
 * @param SDL_keysym keysym
 *      Touche pressee
 */
void TextInput(char* name, SDL_keysym key)
{
    int length = strlen(name);

    // Gestion de l'effacement
    if (length > 0 && key.sym == SDLK_BACKSPACE)
        name[length - 1] = '\0';

    // Gestion des touches
    else if (length < 13)
    {
        int ascii = -1;

        if ((key.sym >= SDLK_a && key.sym <= SDLK_z) ||
            (key.sym >= SDLK_0 && key.sym <= SDLK_9) ||
            key.sym == SDLK_SPACE)
        {
            ascii = key.sym;

            // Gestion de la majuscule
            if ((key.mod & KMOD_SHIFT) && (key.sym >= SDLK_a && key.sym <= SDLK_z))
                ascii -= 32;
        }
        else if (key.sym >= SDLK_KP0 && key.sym <= SDLK_KP9)
            ascii = key.sym - 208;

        if (ascii != -1)
        {
            name[length] = ascii;
            name[length + 1] = '\0';
        }
    }
}

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

                // Sï¿½lection des zones de texte
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

/* Procedure d'affichage des textes
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param TTF_Font* font
 *     Police des textes
 * @param E_GameMode gameMode
 *     Mode de jeu du programme en cours
 * @param E_MenuSelected selected
 *     Selection du menu
 */
void DisplayText(SDL_Surface* window, TTF_Font* font, E_GameMode gameMode, E_MenuSelected selected)
{
    SDL_Color black = {0, 0, 0};
    SDL_Color selectColor = {200, 200, 100};
    SDL_Rect position;

    SDL_Surface *txtPlayer1, *txtPlayer2, *txtColor, *txtBlack, *txtWhite;
    SDL_Surface *txtOption1, *txtOption2, *txtScore;
    SDL_Surface *txtStart, *txtQuit;

    txtBlack = TTF_RenderText_Blended(font, "Noir", black);
    txtWhite = TTF_RenderText_Blended(font, "Blanc", black);
    txtOption1 = TTF_RenderText_Blended(font, "Aide aux mouvements", black);
    txtOption2 = TTF_RenderText_Blended(font, "Pas d'aide", black);
    txtScore = TTF_RenderText_Blended(font, "Score à atteindre :", black);

    txtStart = TTF_RenderText_Blended(font, "Jouer", (selected == START)? selectColor : black);
    txtQuit = TTF_RenderText_Blended(font, "Quitter", (selected == QUIT)? selectColor : black);

    if (gameMode == HUMAN_HUMAN)
    {
        txtPlayer1 = TTF_RenderText_Blended(font, "Joueur 1 :", black);
        txtPlayer2 = TTF_RenderText_Blended(font, "Joueur 2 :", black);
        txtColor = TTF_RenderText_Blended(font, "Couleur du joueur 1 :", black);
    }
    else if (gameMode == HUMAN_AI)
    {
        txtPlayer1 = TTF_RenderText_Blended(font, "Joueur :", black);
        txtPlayer2 = TTF_RenderText_Blended(font, "IA :", black);
        txtColor = TTF_RenderText_Blended(font, "Couleur du joueur :", black);
    }
    else
    {
        txtPlayer1 = TTF_RenderText_Blended(font, "IA 1 :", black);
        txtPlayer2 = TTF_RenderText_Blended(font, "IA 2 :", black);
        txtColor = TTF_RenderText_Blended(font, "Couleur de l'IA 1 :", black);
        txtOption1 = TTF_RenderText_Blended(font, "Match instantané", black);
        txtOption2 = TTF_RenderText_Blended(font, "Match tour par tour", black);
    }

    position.x = 338 - txtPlayer1->w; position.y = 146 - txtPlayer1->h/2;
    SDL_BlitSurface(txtPlayer1, NULL, window, &position);

    position.x = 338 - txtPlayer2->w; position.y = 206 - txtPlayer2->h/2;
    SDL_BlitSurface(txtPlayer2, NULL, window, &position);

    position.x = 338 - txtColor->w; position.y = 266 - txtColor->h/2;
    SDL_BlitSurface(txtColor, NULL, window, &position);

    position.x = 429; position.y = 266 - txtBlack->h/2;
    SDL_BlitSurface(txtBlack, NULL, window, &position);

    position.x = 618; position.y = 266 - txtWhite->h/2;
    SDL_BlitSurface(txtWhite, NULL, window, &position);

    position.x = 91; position.y = 326 - txtOption1->h/2;
    SDL_BlitSurface(txtOption1, NULL, window, &position);

    position.x = 470; position.y = 326 - txtOption2->h/2;
    SDL_BlitSurface(txtOption2, NULL, window, &position);

    position.x = 338 - txtScore->w; position.y = 386 - txtScore->h/2;
    SDL_BlitSurface(txtScore, NULL, window, &position);

    position.x = 308 - txtStart->w/2 - 75; position.y = 470 - txtStart->h/2;
    SDL_BlitSurface(txtStart, NULL, window, &position);

    position.x = 450 + 75 - txtQuit->w/2; position.y = 470 - txtQuit->h/2;
    SDL_BlitSurface(txtQuit, NULL, window, &position);

    SDL_FreeSurface(txtPlayer1);
    SDL_FreeSurface(txtPlayer2);
    SDL_FreeSurface(txtColor);
    SDL_FreeSurface(txtBlack);
    SDL_FreeSurface(txtWhite);
    SDL_FreeSurface(txtOption1);
    SDL_FreeSurface(txtOption2);
    SDL_FreeSurface(txtScore);
    SDL_FreeSurface(txtStart);
    SDL_FreeSurface(txtQuit);
}

/* Procedure de gestion de l'affichage des overlays
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param TTF_Font* font
 *     Police des textes
 * @param E_MenuSelected selected
 *     Selection du menu
 * @param S_GameConfig gameConfig
 *     Structure de configuration du jeu
 */
void DisplayOverlays(SDL_Surface* window, TTF_Font* font, E_MenuSelected selected, S_GameConfig gameConfig)
{
    SDL_Surface *overlays = IMG_Load(DESIGN_PATH "overlays.png");
    SDL_Rect clip, position;

    // Changement de couleur du champ de texte courant du nom du joueur
    clip.x = 40; clip.y = 0; clip.w = 300; clip.h = 40;
    position.x = 379;

    if (selected == PLAYER1)
        position.y = 126;
    else if (selected == PLAYER2)
        position.y = 186;

    if (selected == PLAYER1 || selected == PLAYER2)
        SDL_BlitSurface(overlays, &clip, window, &position);

    // Indication de la couleur et des options
    clip.x = 0; clip.w = 40;

    position.y = 246;
    position.x = (gameConfig.player1Color == BLACK) ? 379 : 568;

    SDL_BlitSurface(overlays, &clip, window, &position);

    position.y = 306;
    position.x = (gameConfig.option) ? 41 : 420;
    SDL_BlitSurface(overlays, &clip, window, &position);

    // Dessin des fleches pour le changement de points
    clip.x = 340; clip.w = 30; position.y = 366;
    if (gameConfig.points > MIN_POINTS)
    {
        // Flï¿½che gauche
        position.x = 444;
        SDL_BlitSurface(overlays, &clip, window, &position);
    }

    clip.x = 370;
    if (gameConfig.points < MAX_POINTS)
    {
        // Flï¿½che droite
        position.x = 584;
        SDL_BlitSurface(overlays, &clip, window, &position);
    }

    // Textes
    SDL_Color black = {0, 0, 0};

    if (strlen(gameConfig.namePlayer1) > 0)
    {
        SDL_Surface *txtNamePlayer1 = TTF_RenderText_Blended(font, gameConfig.namePlayer1, black);
        position.x = 529 - txtNamePlayer1->w/2; position.y = 146 - txtNamePlayer1->h/2;
        SDL_BlitSurface(txtNamePlayer1, NULL, window, &position);
        SDL_FreeSurface(txtNamePlayer1);
    }

    if (strlen(gameConfig.namePlayer2) > 0)
    {
        SDL_Surface *txtNamePlayer2 = TTF_RenderText_Blended(font, gameConfig.namePlayer2, black);
        position.x = 529 - txtNamePlayer2->w/2; position.y = 206 - txtNamePlayer2->h/2;
        SDL_BlitSurface(txtNamePlayer2, NULL, window, &position);
        SDL_FreeSurface(txtNamePlayer2);
    }

    char points[5];
    sprintf(points, "%i", gameConfig.points);

    SDL_Surface *txtPoints = TTF_RenderText_Blended(font, points, black);
    position.x = 529 - txtPoints->w/2; position.y = 386 - txtPoints->h/2;
    SDL_BlitSurface(txtPoints, NULL, window, &position);

    SDL_FreeSurface(txtPoints);
    SDL_FreeSurface(overlays);
}

/* Procedure d'initialisation des noms des joueurs
 * @param S_GameConfig gameConfig
 *     Structure de configuration du jeu
 * @param E_GameMode gameMode
 *     Mode de jeu du programme en cours
 * @param S_AIFunctions* aiFunctions
 *     Structure de stockage des fonctions des bibliotheques
 */
void InitPlayersName(S_GameConfig* gameConfig, E_GameMode gameMode, S_AIFunctions* aiFunctions)
{
    if (gameMode == HUMAN_HUMAN)
    {
        strcpy(gameConfig->namePlayer1, "Joueur 1");
        strcpy(gameConfig->namePlayer2, "Joueur 2");
    }
    else if (gameMode == HUMAN_AI)
    {
        strcpy(gameConfig->namePlayer1, "Joueur");

        char aiName[50];
        aiFunctions[0].AI_InitLibrary(aiName);

        if (strlen(aiName) > 0)
        {
            strncpy(gameConfig->namePlayer2, aiName, 12);
            gameConfig->namePlayer2[13] = '\0';
        }
        else
            strcpy(gameConfig->namePlayer2, "IA\0");
    }
    else
    {
        char aiName1[50];
        aiFunctions[0].AI_InitLibrary(aiName1);

        if (strlen(aiName) > 0)
        {
            strncpy(gameConfig->namePlayer1, aiName1, 12);
            gameConfig->namePlayer1[13] = '\0';
        }
        else
            strcpy(gameConfig->namePlayer1, "IA 1\0");

        char aiName2[50];
        aiFunctions[1].AI_InitLibrary(aiName2);

        if (strlen(aiName) > 0)
        {
            strncpy(gameConfig->namePlayer2, aiName2, 12);
            gameConfig->namePlayer2[13] = '\0';
        }
        else
            strcpy(gameConfig->namePlayer2, "IA 2\0");
    }
}

/* Procedure de gestion de l'affichage du menu
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param E_GameMode gameMode
 *     Mode de jeu du programme en cours
 * @param S_AIFunctions* aiFunctions
 *     Structure de stockage des fonctions des bibliotheques
 */
void DisplayMenu(SDL_Surface* window, E_GameMode gameMode, S_AIFunctions* aiFunctions)
{
    SDL_Surface *menu_bg = IMG_Load(DESIGN_PATH "menu.png");
    TTF_Font *font = TTF_OpenFont(DESIGN_PATH "font.ttf", 25);

    SDL_Rect position;
    position.x = 0; position.y = 0;

    E_MenuSelected selected = NONE;
    S_GameConfig gameConfig;

    gameConfig.player1Color = BLACK;
    gameConfig.option = 0;
    gameConfig.points = 5;
    InitPlayersName(&gameConfig, gameMode, aiFunctions);

    SDL_BlitSurface(menu_bg, NULL, window, &position);
    DisplayText(window, font, gameMode, selected);
    DisplayOverlays(window, font, selected, gameConfig);

    int finish = 0;
    SDL_Event event;

    while(!finish)
    {
        // Affichage
        if (event.type == SDL_MOUSEBUTTONUP ||
            event.type == SDL_MOUSEBUTTONDOWN ||
            event.type == SDL_KEYUP)
        {
            SDL_BlitSurface(menu_bg, NULL, window, &position);
            DisplayText(window, font, gameMode, selected);
            DisplayOverlays(window, font, selected, gameConfig);
        }

        SDL_Flip(window);

        // Gestion des evenements
        E_MenuSelected button = EventsMenu(&event, &gameConfig, &selected);

        if (button == START)
            finish = DisplayBoard(window, gameMode, aiFunctions, gameConfig);
        else if (button == QUIT)
            finish = 1;

        SDL_Delay(5);
    }

    TTF_CloseFont(font);
    SDL_FreeSurface(menu_bg);
}
