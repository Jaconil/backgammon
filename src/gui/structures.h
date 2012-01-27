#ifndef _STRUCTURES
#define _STRUCTURES

// Definition des structures et des enumerations de l'interface
#include "../backgammon.h"

// Styles de jeu
typedef enum
{
    HUMAN_HUMAN,
    HUMAN_AI,
    AI_AI,
    ERROR
} E_GameMode;

// Stockage des API et des pointeurs de fonctions pour les 2 IA
typedef struct
{
    void* ai;
    pfInitLibrary  AI_InitLibrary;
    pfStartMatch   AI_StartMatch;
    pfStartGame    AI_StartGame;
    pfEndGame      AI_EndGame;
    pfEndMatch     AI_EndMatch;
    pfDoubleStack  AI_DoubleStack;
    pfTakeDouble   AI_TakeDouble;
    pfMakeDecision AI_MakeDecision;
} S_AIFunctions;

// Partie du menu qui est sélectionee
typedef enum
{
    PLAYER1,
    PLAYER2,
    START,
    QUIT,
    NONE
} E_MenuSelected;

// Couleur des joueurs
typedef enum
{
    BLACK,
    WHITE
} E_PlayerColor;

// Configuration du match
typedef struct
{
    char namePlayer1[14];
    char namePlayer2[14];
    E_PlayerColor player1Color;
    int option;
    int points;
    E_GameMode mode;
    S_AIFunctions* aiFunctions;
} S_GameConfig;

// Enumeration des etapes du jeu
typedef enum
{
    WAITING_FIRST_ROLL,     // Attente du premier lancer
    FIRST_ROLL_POPUP,       // Message indiquant le premier joueur
    SELECT_ZONE_SRC,        // Selection d'une fleche de depart
    SELECT_ZONE_DST,        // Selection d'une flehe d'arrivee
    WAITING_ROLL_DBL,       // Attente d'un lancer ou doubla
    DOUBLE_POPUP,           // Message informant un double
    WAITING_ROLL,           // Attente d'un lancer
    PASS_POPUP,             // Message indiquant que le joueur passe son tour
    FINISH_GAME_POPUP,      // Message indiquant la fin d'une partie
    FINISH_MATCH_POPUP      // Message indiquant la fin d'un match
} E_Stage;

// Texte du plateau selectionne
typedef enum
{
    BUTTON1,
    BUTTON2,
    NONE_BOARD,
    QUIT_BOARD,
    MENU_BOARD
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

    // Coups restants pour chaque dé
    unsigned int useDie1, useDie2;

    unsigned int scoreP1, scoreP2;

    unsigned int stake;
    EPlayer cubeOwner;

    S_GameConfig gameConfig;

    // Etape courante du jeu
    E_Stage currentStage;

    // Joueur courant
    EPlayer currentPlayer;

    // Fleche selectionnee
    unsigned int currentZone;

    E_BoardSelected selected;

    // Indique qu'on doit raffraichir l'affichage sans clic
    int refresh;

    // Erreurs des IA
    unsigned int aiErrors[2];

} S_GameState;

#endif