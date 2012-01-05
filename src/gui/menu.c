#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "cste.h"
#include "library.h"
#include "menu.h"

/* Fonction de gestion des evenements du menu
 * @param SDL_Event* event
 *     Evenements de la fenetre
 * @param int* finish
 *     Indicateur de la fin du programme
 */
void EventsMenu(SDL_Event* event, int* finish)
{
    SDL_WaitEvent(event);
    switch(event->type)
    {
        case SDL_QUIT:
            *finish = 1;
    }
}

/* Fonction d'affichage des textes
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param TTF_Font* font
 *     Police des textes
 * @param E_GameMode gameMode
 *     Mode de jeu du programme en cours
 */
void DisplayText(SDL_Surface* window, TTF_Font* font, E_GameMode gameMode)
{
    SDL_Color black = {0, 0, 0};        
    SDL_Rect position;   
    
    SDL_Surface *txtPlayer1, *txtPlayer2, *txtColor, *txtBlack, *txtWhite;
    SDL_Surface *txtOption1, *txtOption2, *txtScore;
    
    txtBlack = TTF_RenderText_Blended(font, "Noir", black);
    txtWhite = TTF_RenderText_Blended(font, "Blanc", black);
    txtOption1 = TTF_RenderText_Blended(font, "Aide aux mouvements", black);
    txtOption2 = TTF_RenderText_Blended(font, "Pas d'aide", black);
    txtScore = TTF_RenderText_Blended(font, "Points pour gagner :", black);
    
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
}

/* Fonction de gestion de l'affichage du menu
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param E_GameMode gameMode
 *     Mode de jeu du programme en cours
 * @param S_AIFunctions aiFunctions
 *     Structure de stockage des fonctions des bibliotheques
 */
void DisplayMenu(SDL_Surface* window, E_GameMode gameMode, S_AIFunctions* aiFunctions)
{
    SDL_Surface *menu_bg = IMG_Load(DESIGN_PATH "menu.png");
    
    SDL_Rect position;    
    position.x = 0; position.y = 0;    
    SDL_BlitSurface(menu_bg, NULL, window, &position);
    
    TTF_Font *font = TTF_OpenFont(DESIGN_PATH "font.ttf", 25);
    
    DisplayText(window, font, gameMode);
    
    int finish = 0;
    SDL_Event event;
    
    while(!finish)
    {
        EventsMenu(&event, &finish);
        
        // Affichage
        SDL_Flip(window);
    }
    
    SDL_FreeSurface(menu_bg);
}
