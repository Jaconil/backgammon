#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "cste.h"
#include "structures.h"
#include "gui.h"

/* Fonction d'initialisation de la SDL,de la fenetre et des surfaces
 * @param SDL_Surface* window
 *     Surface de la fenetre du programme
 * @param SDL_Surface** icon
 *     Surface de l'icone du programme
 * @return int
 *     1 si aucune erreur, 0 sinon
 */
int InitWindow(SDL_Surface** window, SDL_Surface** icon)
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        return 0;
    }

    // Chargement de l'icone
    *icon = IMG_Load(DESIGN_PATH "icon.png");
    SDL_WM_SetIcon(*icon, NULL);

    // Chargement de la fenetreelse
    *window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (*window == NULL)
    {
        fprintf(stderr, "Erreur d'ouverture du mode vidéo : %s\n", SDL_GetError());
        return 0;
    }

    // Activation du mode Unicode
    SDL_EnableUNICODE(1);

    SDL_WM_SetCaption("Backgammon", NULL);

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'ouverture de TTF_Init : %s\n", TTF_GetError());
        return 0;
    }

    return 1;
}

/* Procédure pour liberer les ressources graphiques
 * @param SDL_Surface* icon
 *     Surface de l'icone du programme
 */
void FreeWindow(SDL_Surface* icon)
{
    // Desallocation de l'icone
    SDL_FreeSurface(icon);

    TTF_Quit();
    SDL_Quit();
}

/* Fonction qui indique si l'utilisateur a clique sur un rectangle
 * @param SDL_Event* event
 *     Evenements de la fenetre
 * @param int x
 *     Position x du rectangle
 * @param int y
 *     Position y du rectangle
 * @param int w
 *     Largeur du rectangle
 * @param int h
 *     Hauteur du rectangle
 * @return int
 *     1 si le clic est dans le rectangle, 0 sinon
 */
int ClickRect(SDL_Event* event, int x, int y, int w, int h)
{
    return (event->button.x >= x && event->button.x <= x + w &&
            event->button.y >= y && event->button.y <= y + h);
}

/* Fonction qui renvoie la zone sur laquelle on a clique
 * @param SDL_Event* event
 *     Evenements de la fenetre
 * @return int
 *     Numéro de la flèche cliquee, ou -1 si aucune fleche cliquee
 */
int ClickZone(SDL_Event* event)
{
    int zone = -1;
    int x = event->button.x;
    int y = event->button.y;

    // Gestion des fleches
    int i, xmin = 0, xmax = 0;

    for (i=0; i<12; i++)
    {
        xmin = BORDER + i * ZONE_W;
        if (i > 5)
            xmin += 2 * BORDER;

        xmax = xmin + ZONE_W;

        if (x >= xmin && x <= xmax)
        {
            if (y >= BORDER && y <= BORDER + ZONE_H + 10)
                zone = i + 12;
            else if (y >= BOTTOM - BORDER - ZONE_H - 10 && y <= BOTTOM - BORDER)
                zone = 11 - i;
        }
    }

    // Gestion des prisonniers
    if ((x >= 323 && x <= 323 + CHECKER_W) && (y >= 185 && y <= 185 + CHECKER_W))
        zone = EPos_BarP1;
    else if ((x >= 323 && x <= 323 + CHECKER_W) && (y >= 265 && y <= 265 + CHECKER_W))
        zone = EPos_BarP2;

    // Gestion des zones de sortie
    if ((x >= 697 && x <= 697 + OUT_W) && (y >= BORDER && y <= BORDER + OUT_H))
        zone = EPos_OutP2;
    else if ((x >= 697 && x <= 697 + OUT_W) && (y >= 295 && y <= 295 + OUT_H))
        zone = EPos_OutP1;

    return zone;
}

/* Procedure de dessin de bouton
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param int x
 *     Abscisse du centre du bouton
 * @param int y
 *     Ordonnee du centre du bouton
 * @param char* label
 *     Texte du bouton
 * @param int select
 *     1 si le bouton est selectionne, 0 sinon
 */
void DisplayButton(SDL_Surface* window, int x, int y, char* label, int select)
{
    SDL_Surface *overlays = IMG_Load(DESIGN_PATH "overlays.png");
    SDL_Rect clip, position;
    TTF_Font *font = TTF_OpenFont(DESIGN_PATH "board.ttf", 20);
    SDL_Color black = {0,0,0}, selectColor = {200, 200, 100};
    SDL_Surface *txtButton = NULL;

    clip.x = 80; clip.y = 80; clip.w = 100; clip.h = 30;
    position.x = x - clip.w/2; position.y = y - clip.h/2;
    SDL_BlitSurface(overlays, &clip, window, &position);

    txtButton = TTF_RenderText_Blended(font, label, select ? selectColor : black);
    position.x = x - txtButton->w/2; position.y = y - txtButton->h/2;
    SDL_BlitSurface(txtButton, NULL, window, &position);

    TTF_CloseFont(font);
    SDL_FreeSurface(overlays);
}
