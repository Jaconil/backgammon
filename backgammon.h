/////////// Strutures � utiliser

// Repr�sente un joueur
typedef enum
{
	EPlayer1,	// Joueur 1 -> le joueur courant pour la strat�gie
	EPlayer2,	// Joueur 2
} EPlayer;

// Structure de repr�sentation d'une zone du jeu
typedef struct
{
	EPlayer player;				// Joueur propri�taire des pions
	unsigned int nb_checkers;	// Nombre de pion du joueur sur la zone
} SZone;

// Enumeration des zones pour le tableau points
typedef enum
{
	EPos_1=0,
	EPos_2,
	EPos_3,
	EPos_4,
	EPos_5,
	EPos_6,
	EPos_7,
	EPos_8,
	EPos_9,
	EPos_10,
	EPos_11,
	EPos_12,
	EPos_13,
	EPos_14,
	EPos_15,
	EPos_16,
	EPos_17,
	EPos_18,
	EPos_19,
	EPos_20,
	EPos_21,
	EPos_22,
	EPos_23,
	EPos_24,
	EPos_OutP1,
	EPos_BarP1,
	EPos_OutP2,
	EPos_BarP2
} EPosition;

// Structure de repr�sentation du jeu avec son plateau et les d�s
typedef struct
{
	SZone zones[28];			// Tableau contenants les zones o� les pions peuvent se trouver, index�s par des EPosition
	unsigned int die1,die2;		// Les d�s de jeu
	unsigned int score,scoreP2;	// Les scores des joueurs
	unsigned int stake;			// Mise courante de la partie
} SGameState;

// Structure de repr�sentation d'un mouvement
typedef struct
{
	EPosition src_point;
	EPosition dest_point;
} SMove;

/////////// Focntions � utiliser
// Les prototypes en commentaires sont � impl�menter dans la librairie
// Les types pointeur de fonction dans l'application

/**
 * Initialiser la librairie
 * @param char name[50]
 *	nom associ� � la librairie
 */
//void InitLibrary(char name[50]);
typedef void (*pfInitLibrary)(char[50]);

/**
 * Initialiser l'IA pour un match
 * @param const unsigned int target_score
 *	score cible pour gagner un match
 */
//void StartMatch(const unsigned int target_score);
typedef void (*pfStartMatch)(const unsigned int);

/**
 * Initialiser l'IA pour une manche (d'un match)
 */
//void StartGame();
typedef void (*pfStartGame)();

/**
 * Fin d'une manche (d'un match)
 */
//void EndGame();
typedef void (*pfEndGame)();

/**
 * Fin d'un match
 */
//void EndMatch();
typedef void (*pfEndMatch)();

/**
 * Doubler la mise
 * @param const SGameState * const gameState
 *	l'�tat du jeu courant
 * @return int
 *	vrai si on propose de doubler : faux sinon
 */
//int DoubleStack(const SGameState * const gameState);
typedef int (*pfDoubleStack)(const SGameState * const);

/**
 * Accepter ou refuser la nouvelle mise
 * @param const SGameState * const gameState
 *	l'�tat du jeu courant
 * @return int
 *	vrai si on accepte la nouvelle mise ; faux sinon
 */
//int TakeDouble(const SGameState * const gameState);
typedef int (*pfTakeDouble)(const SGameState * const);

/**
 * Prise de d�cision de la part de l'IA
 * @param const SGameState * const gameState
 *	l'�tat du jeu courant
 * @param SMove moves[4]
 *	tableau des mouvements � effectuer par l'IA
 * @param unsigned int lastTimeError
 *	vrai si la derni�re action a caus�e une erreur
 */
//void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError);
typedef void (*pfMakeDecision)(const SGameState * const, SMove[4], unsigned int);
