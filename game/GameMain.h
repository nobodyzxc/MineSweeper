#ifndef _GAMEMAIN_H_
#define _GAMEMAIN_H_
int **gameStart(int mapLN , int mapWD , int bomb_num);
void printMap(int);
void DebugNum(int);
void run(char , int , int);
int inRange(int , int);
int ArroundNum(int , int);
bool CheckIfWin(void);
void GameOver(int , int);
/* recursively open */
void expand(int , int);

#define BOMB            (1 << 0)
#define QUES_MARK       (1 << 1)
#define FLAG            (1 << 2)
#define OPEN            (1 << 3)
#define isNon(x)        ((!hasBOMB(x)) && (!hasFLAG(x)) && (!hasQUES(x)))
#define hasBOMB(x)      (x & BOMB)
#define hasQUES(x)      (x & QUES_MARK)
#define hasFLAG(x)      (x & FLAG)
#define hasOPENED(x)    (x == OPEN)
#define hasNoMark(x)    (!(hasQUES(x) || hasFLAG(x) || hasOPENED(x)))
#define setBOMB(x)      (x |= BOMB)
#define setQUES(x)      (unsetFLAG(x) , x |= QUES_MARK)
#define setFLAG(x)      (unsetQUES(x) , x |= FLAG)
#define setOPEN(x)      (x = OPEN) 
#define unsetQUES(x)    (x &= (~QUES_MARK))
#define unsetFLAG(x)    (x &= (~FLAG))

#endif
