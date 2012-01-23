void initGameBoard(SZone* gameBoard);

#define TRUE 1
#define FALSE 0

//*******************************************//
void initTableauSMoveLocal(SMove tableau[4]);
void rafraichitFlecheJouable(const SGameState * const gameState);
void copierTableauSMove(SMove source[4], SMove destination[4]);
int lastChecker();
int min(int a, int b);
int max(int a, int b);
int nbMove(int a, int b);
int nombrePionBar(const SGameState * const gameState);
int distanceToWin(const SGameState * const gameState);
int possibleCase(const SGameState * const gameState, int source);
int mouvementNormalPossible(const SGameState * const gameState, int source, int distance);
int mouvementPossible(const SGameState * const gameState, int source, int distance);
void effectuerUnDeplacementTest(SGameState * plateau, int source, int de);
void enregistrerUnDeplacement(int i, int source, int de);
void decision2Move(const SGameState * const gameState);
void decision4Move(const SGameState * const gameState);
void redirectionEnFonctionDesDes (const SGameState * const gameState);
int calculerCoupRestant(const SGameState * const gameState, EPlayer unJoueur);
int pratiquementGagner(double deplacementRestantP1, double deplacementRestantP2);
void InitLibrary(char name[50]);
void StartMatch(const unsigned int target_score);
void StartGame();
void EndGame();
int DoubleStack(const SGameState * const gameState);
int TakeDouble(const SGameState * const gameState);
void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError);
