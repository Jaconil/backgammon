void initGameBoard(SZone* gameBoard);

#define TRUE 1
#define FALSE 0

/**
 *Permet d'initialiser le tableau de mouvement
 *@param SMove tableau[4]
 *	tableau de mouvement
 */
void initTableauSMoveLocal(SMove tableau[4]);

/**
 *Permet de rafraichir le variable globale enregistrant le jeu
 *@param const SGameState * const gameState
 *	l'état du jeu courant
 */
void rafraichitFlecheJouable(const SGameState * const gameState);

/**
 *Copie le tableau source vers le tableau destination
 *@param SMove source[4]
 *	tableau source à copier
 *@param SMove destination[4]
 *	tableau destination qui sera une copie du tableau source
 */
void copierTableauSMove(SMove source[4], SMove destination[4]);

/**
 *Renvoie la position du dernier pion pour le joueur 1 dans le jeu
 *@param const SGameState * const gameState
 *	l'état du jeu courant
 *@return int
 *	position du dernier pion du joueur 1
 */
int lastChecker(const SGameState * const gameState);

/**
 *Calcul le nombre de pions qui ne sont pas sortis du jeu. Les joueurs prisonniers sont comptés comme des pions en jeu
 *@param const SGameState * const gameState
 *	l'état du jeu courant
 *@return int
 *	nombre de pion en jeu restant incluant les prisonniers
 */
int nbPionEnJeu(const SGameState * const gameState);

/**
 *Calcul le nombre de fleche jouable restante
 *@param const SGameState * const gameState
 *	l'état du jeu courant
 *@return int
 *	retourne le nombre de fleche jouable
 */
int nbBlotEnJeu(const SGameState * const gameState);

/**
 *Calcul du minimum
 *@param int a
 *	premier entier à comparer
 *@param int b
 *	second entier à comparer
 *@return int
 *	retourne le minimum
 */
int min(int a, int b);

/**
 *Calcul du maximum
 *@param int a
 *	premier entier à comparer
 *@param int b
 *	second entier à comparer
 *@return int
 *	retourne le maximum
 */
int max(int a, int b);

/**
 *Calcul le nombre de mouvements autorisés
 *@param int a
 *	premier dé
 *@param int b
 *	second dé
 *@return int
 *	nombre de mouvements possibles
 */
int nbMove(int a, int b);

/**
 *Retourne le numéro de la stratégie
 *@param const SGameState * const gameState
 *	l'état courant du jeu
 *@param int lastTimeError
 *	paramètre déterminant si il y a eu une erreure ou pas
 */
void calculerStrategie(const SGameState * const gameState, int lastTimeError);

/**
 *permet de déterminer quel plateau est le meilleur
 *@param SGameState* plateurATester
 *	plateau test ou les movements à comparer sont enregistrer
 *@param const SGameState * const gameState
 *	l'état courant du jeu
 *@return int
 *	retourne le numéro de la stratégie la plus efficace
 */
int meilleurPlateau(SGameState* plateauATester, const SGameState * const gameState)

/**
 *Retourne les cases possibles
 *@param const SGameState * const gameState
 *	l'état courant du jeu
 *@param int source
 *	flèche source du mouvement
 *@return int
 *	retourne 1 si le mouvement est possible 0 sinon
 */
int possibleCase(const SGameState * const gameState, int source);

/**
 *Détermine si un mouvement est possible ou pas
 *&param const SGameState * const gameState
 *	l'état courant du jeu
 *@param int source
 *	flèche de départ du mouvement
 *@param int distance
 *	flèche d'arrivée du mouvement
 *@return int
 *	retourne 1 si le mouvement est possible, 0 sinon
 */
int mouvementNormalPossible(const SGameState * const gameState, int source, int distance);

/**
 *détermine si un mouvement est possible
 *@param const SGameState * const gameState
 *	l'état du jeu courant
 *@param int source
 *	numéro de la flèche source
 *@param int distance
 *	numéro de la flèche destination
 *@return int
 *	1 si le mouvement est possible, 0 sinon
 */
int mouvementPossible(const SGameState * const gameState, int source, int distance);

/**
 *fonction qui permet d'effectuer des tests sur les mouvements sur un plateau de test
 *@param SGameState * plateau
 *	plateau test qui est une copie du plateau de jeu courant
 *@param int source
 *	fleche source du mouvement
 *@param int de
 *	dé à utilisé
 */
void effectuerUnDeplacementTest(SGameState * plateau, int source, int de);

/**
 *fonction qui effectue les mouvements
 *@param SGameState * plateauEnCours
 *	plateau de test
 *@param const SGameState * const gameState
 *	l'état du jeu courant
 */
void effectuerLesMouvements(SGameState * plateauEnCours, const SGameState * const gameState);

/**
 *permet d'enregistrer un déplacement
 *@param int i
 *	numéro du mouvement
 *@param int source
 *	numéro de la fleche source
 *@param int de
 *	dé utilisé
 */
void enregistrerUnDeplacement(int i, int source, int de);

/**
 *fonction qui détermine les deux mouvements possibles si le nombre de mouvements autorisés est de 2
 *@param const SGameState * const gameState
 *	l'état courant du jeu
 */
void decision2Move(const SGameState * const gameState);

/**
 *fonction qui détermine les deux mouvements possibles si le nombre de mouvements autorisés est de 4
 *@param const SGameState * const gameState
 *	l'état courant du jeu
 */
void decision4Move(const SGameState * const gameState);

/**
 *permet de lancer la fonction déterminant les mouvements en fonction des dés
 *@param const SGameState * const gameState
 *	l'état du jeu courant
 */
void redirectionEnFonctionDesDes (const SGameState * const gameState);

/**
 *retourne le nombre de mouvement restant d'un joueur
 *@param const SGameState * const gameState
 *	l'état du jeu courant
 *@param EPlayer unJoueur
 *	le joueur dont on veut le nombre de déplacement restant
 *@return int
 *	le nombre de mouvements restants
 */
int calculerCoupRestant(const SGameState * const gameState, EPlayer unJoueur);

/**
 *Détermine si le joueur 1 va gagner
 *@param double deplacementRestantP1
 *	nombre de déplacements restants du joueur 1
 *@param double deplacementRestantP2
 *	nombre de déplacements restants du joueur 2
 *@return int
 *	si le joueur 1 est pratiquement sur de gagner renvoie 1, 0 sinon
 */
int pratiquementGagner(double deplacementRestantP1, double deplacementRestantP2);

/**
 *initialiser la librairie
 *@param char name[50]
 *	nom associé à la librairie
 */
void InitLibrary(char name[50]);

/**
 *initialiser l'IA pour le match
 *@param const unsigned int target_score
 * score cible pour gagner le match
 */
void StartMatch(const unsigned int target_score);

/**
 *Initialiser l'IA pour une manche (d'un match)
 */
void StartGame();

/**
 *Fin du match
 */
void EndGame();

/**
 *Double la mise
 *@param const SGameState * const gameState
 *	l'état du jeu courant
 *@return int
 *	1 si on propose de doubler, 0 sinon
 */
int DoubleStack(const SGameState * const gameState);

/**
 *Accepte ou refuse la mise de l'autre joueur
 *@param const SGameState * const gameState
 *	l'état du jeu courant
 *@return int
 *	1 si on accepte de doubler, 0 sinon
 */
int TakeDouble(const SGameState * const gameState);

/**
 *Prise de décision de la part de l'IA
 *@param const SGameState * const gameState
 *	l'état du jeu courant
 *@param SMove moves [4]
 *	tableau des mouvements à effectuer par l'IA
 *@param unsigned int lastTimeError
 *	vrai si la dernière action a causée une erreur
 */
void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError);