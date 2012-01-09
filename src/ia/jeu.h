void initGameBoard(SZone* gameBord);



//*******************************************//
int min(int a, int b);
int max(int a, int b);
EPosition minBeEaten(const SZone* zones, EPosition a, EPosition b);
int nbMove(int a, int b);
int distanceToWin(const SZone* zones);
EPosition LastChecker1(const SZone* zones);
EPosition LastChecker2(const SZone* zones, int N);
int possibleCase(SZone arrow);
double strikeJet(int distance);
double probabilityOfBeEaten(const SZone* zones, EPosition position);
void move(SMove* moves, EPosition src, EPosition dest);
int getCheckerIn(const SZone* zones, SMove* moves, int* die1, int* die2);
int checkersInHomeBoard(const SZone* zones);
int getCheckerOut(const SZone* zones, SMove* moves, int* die1, int* die2);
void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError);
