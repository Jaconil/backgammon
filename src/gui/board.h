#ifndef _BOARD
#define _BOARD

// Enumeration des etapes du jeu
typedef enum
{
    WAITING_FIRST_ROLL,
    FIRST_ROLL_POPUP,
    SELECT_ZONE_SRC,
    SELECT_ZONE_DST,
    WAITING_ROLL_DBL,
    DOUBLE_POPUP,
    WAITING_ROLL
} E_Stage;

// Texte du plateau selectionne
typedef enum
{
    ROLL,
    NONE_BOARD,
    QUIT_BOARD
} E_BoardSelected;

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

    E_Stage currentStage;
    EPlayer currentPlayer;
    unsigned int currentZone;

    E_BoardSelected selected;
} S_GameState;

void InitGameState(S_GameState* gameState, S_GameConfig gameConfig);
void DisplayCheckers(SDL_Surface* window, S_GameState gameState);
void DisplayBoardOverlays(SDL_Surface* window, S_GameState gameState);

int DisplayBoard(SDL_Surface* window, E_GameMode gameMode, S_AIFunctions* aiFunctions, S_GameConfig gameConfig);

E_BoardSelected EventsBoard(SDL_Event* event, S_GameState* gameState);

void RollDice(S_GameState* gameState);
#endif
