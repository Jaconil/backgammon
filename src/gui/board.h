#ifndef _BOARD
#define _BOARD

// GameState personnalise, reprennant celle de l'interface en rajoutant les attributs manquants
typedef struct
{
    // Joueur1 : EPos_24 --> EPos_1
    // Joueur2 : EPos_1  --> EPos_24

    // EPos_1 = en bas a droite
    // Epos_24 = en haut a droite
    SZone zones[28];

    unsigned int die1, die2;
    unsigned int scoreP1, scoreP2;

    unsigned int stake;
    EPlayer cubeOwner;

    S_GameConfig gameConfig;

} S_GameState;

void InitGameState(S_GameState* gameState, S_GameConfig gameConfig);

void DisplayCheckers(SDL_Surface* window, S_GameState gameState);

int DisplayBoard(SDL_Surface* window, E_GameMode gameMode, S_AIFunctions* aiFunctions, S_GameConfig gameConfig);

#endif