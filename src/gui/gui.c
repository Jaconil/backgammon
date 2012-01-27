#include <stdio.h>
#include <stdarg.h>

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

    // Chargement de SDL_TTF
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

    // Gestion des touches a-z et 0-9
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
    TTF_Font *font = TTF_OpenFont(DESIGN_PATH "board.ttf", 20);

    SDL_Rect clip, position;
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

/* Procedure de dessin de bouton
 * @param SDL_Surface* window
 *     Surface de la fenetre
 * @param int nb
 *     Nombre de lignes
 * @param char* ...
 *     Lignes de texte
 */
void DisplayPopup(SDL_Surface* window, int nb, ...)
{
    SDL_Surface *overlays = IMG_Load(DESIGN_PATH "overlays.png");
    TTF_Font *font = TTF_OpenFont(DESIGN_PATH "board.ttf", 20);

    SDL_Rect clip, position;
    SDL_Color black = {0,0,0};
    SDL_Surface *txtPopup = NULL;
    int i;
    char* line = NULL;

    va_list lines;
    va_start(lines, nb);

    // Affichage du fond
    clip.x = 180; clip.y = 80; clip.w = 250; clip.h = 200;
    position.x = CENTER_X - clip.w/2; position.y = CENTER_Y - clip.h/2;
    SDL_BlitSurface(overlays, &clip, window, &position);

    position.y = 180;

    // Affichage du texte
    for (i=0; i<nb; i++)
    {
        line = (char*)va_arg(lines, char*);
        txtPopup = TTF_RenderText_Blended(font, line, black);
        position.x = CENTER_X - txtPopup->w/2;

        if (i > 0)
            position.y += txtPopup->h;

        SDL_BlitSurface(txtPopup, NULL, window, &position);
    }

    va_end(lines);

    TTF_CloseFont(font);
    SDL_FreeSurface(overlays);
}