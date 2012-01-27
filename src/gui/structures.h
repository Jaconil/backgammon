#ifndef _STRUCTURES
#define _STRUCTURES

// Definition des structures et des enumerations
#include "backgammon.h"

// Style de jeu
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

// Partie du menu qui est sélectionée
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
    WAITING_FIRST_ROLL,
    FIRST_ROLL_POPUP,
    SELECT_ZONE_SRC,
    SELECT_ZONE_DST,
    WAITING_ROLL_DBL,
    DOUBLE_POPUP,
    WAITING_ROLL,
    PASS_POPUP,
    FINISH_GAME_POPUP,
    FINISH_MATCH_POPUP
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

    E_Stage currentStage;
    EPlayer currentPlayer;
    unsigned int currentZone;

    E_BoardSelected selected;
} S_GameState;

#endif