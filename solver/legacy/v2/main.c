#include<stdio.h>
#include<windows.h>
#include<Winuser.h>
#include<Windows.h>
#include<process.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<time.h>
#include<memory.h>

#include "game.h"
int game_plat_possible;
int left_mine_unknown;
int out_to_rand = 0;

struct notation { float num; int exp; };

int map[MAPHI][MAPWD] = {0} ;
int rsnMap[MAPHI][MAPWD];
//exh means exhausted ,
//the cell has been (addFlgArd or expand) or not
int exh[MAPHI][MAPWD] = {0};
int border[MAPHI][MAPWD] = {0};
struct notation dfs_count[MAPHI][MAPWD];
float flags = FLAGNUM;
float dfs_flags = FLAGNUM;

POINT* adjPts(POINT , int);

int need_flag(void);
int direct();
int dirSng(POINT);
void addFlgArd(POINT);
int rsning(POINT);
int dfs_rsning(void);
int suppose(int);
int resuppose(void);
void dfs(POINT);
void gessClk(void);
int adjCnt(POINT , int , int mata[][MAPWD]);
int adjCntMrk(POINT pt , int mat[][MAPWD]);
int rsnExpn(POINT , int);
void initMouse(void);
void Gaming(void);
int cntMap(int , int matrix[][MAPWD]);
int adjPtNum(POINT);

#include "mineApi.c"
#include "dirty.c"

POINT *adjPts(POINT pt , int ptIdx){

    POINT *rtnPtr = (POINT*)malloc(sizeof(POINT)) ,
          d[] = { {0 , -1} , {1 , -1} , {1 , 0} , {1 , 1} ,
              {0 , 1} , {-1 , 1} , {-1 , 0} , {-1 , -1} };

    int i , idx = 0 , ptNum = adjPtNum(pt);

    if(idx == ptNum) return NULL;

    for(i = 0 ; i < 8 ; i++){
        rtnPtr->x = pt.x + d[i].x;
        rtnPtr->y = pt.y + d[i].y;
        if(PTINMAP(*rtnPtr)){
            if(idx == ptIdx) return rtnPtr;
            idx++;
        }
    }
    return NULL;
}

void initMap(){
    int i , j;
    for(i = 0 ; i< MAPHI ; i++){
        for(j = 0 ; j<MAPWD ; j++){
            map[i][j] = 0;
            rsnMap[i][j] = 0;
            exh[i][j] = border[i][j] = 0;
        }
    }
}

int main(void){

    srand(time(NULL));
    setFormLoc();
    showGamePane();
    initMouse();
    initMap();

//    analyMap(true);
//    printTab(map);
    return 0;

    POINT pt = {15 , 8};
    click(pt , LEFT);
    analyAftClk(pt , LEFT);
    printTab(map);
    flags = (float)(FLAGNUM - cntMap(FLG , map));
    Gaming();

    return 0;
}
void Gaming(void){
    int i , j , modify = 0;

    puts("gaming");

    while(1){

        int pass = 1;
        if(cntMap(UNK , map) == MAPHI * MAPWD && pass){
            pass = 0;
            gessClk();
            analyMap(true);
            continue;
        }
        puts("while beg");
        if(direct()) continue;
        puts("out direct");
        if(gameState() != RUN) return ;

        if(suppose(FLG)) continue;

        if(suppose(SAF)) continue;

        if(gameState() != RUN) return ;

        if(cntMap(UNK , map) == 0) break;

        //        resuppose();
        puts("game ooo");
        printWithExh(map);
        puts("should exit but try dir again");
        if(direct()) continue;
        puts("");
        printWithExh(map);
        exit(0);

    }
    puts("break");
    return ;
}

int direct(){
    puts("dir beg");
    int y , x , modify = 0;
    for(y = 0 ; y < MAPHI ; y++){
        for(x = 0 ; x < MAPWD ; x++){
            POINT pt = {x , y};
            if(dirSng(pt)){
                printf("(%d %d) let modify\n" , pt.y , pt.x);
                modify = 1;
            }
        }
    }
    puts("dir end");
    return modify;
}

int dirSng(POINT pt){
    int modify = 0;
    if(!exh[pt.y][pt.x]){
        if(HASBOMB(map , pt)){
            int adjUnkNum = adjCnt(pt , UNK , map);
            int adjFlgNum = adjCnt(pt , FLG , map);
            if(adjUnkNum == 0
                    && PTON(pt , map) == adjFlgNum){
                printf("(%d %d) exh\n" , pt.y , pt.x);
                PTON(pt , exh) = true;
            }
            else if(adjFlgNum == map[pt.y][pt.x]){
                printf("(%d %d) expand\n" , pt.y , pt.x);
                modify = 1;
                click(pt , BOTH);
                PTON(pt , exh) = true;
                analyAftClk(pt , BOTH);
            }
            else if(adjUnkNum == map[pt.y][pt.x] - adjFlgNum){
                printf("(%d %d) flgard\n" , pt.y , pt.x);
                modify = 1;
                addFlgArd(pt);
                PTON(pt , exh) = true;
            }
        }
    }
    return modify;
}

void addFlgArd(POINT pt){
    POINT *adjptr;
    int ptIdx = 0;
    while(adjptr = adjPts(pt , ptIdx++)){
        if(map[adjptr->y][adjptr->x] == UNK){
            click(*adjptr , RIGHT);
        }
        free(adjptr);
    }
    return ;
}

void cpMapToRsnMap(){
    int y , x;
    for(y = 0; y < MAPHI ; y++)
        for(x = 0 ; x < MAPWD ; x++)
            rsnMap[y][x] = map[y][x];
}

int suppose(int what){
    int i , j , rtn = 0;

    puts("suppose begin");

    for(i = 0 ; i < MAPHI ; i++)
        for(j = 0 ; j < MAPWD ; j++){
            cpMapToRsnMap();
            POINT pt = {j , i};
            int mrks = adjCntMrk(pt , map);
            if(PTON(pt , map) == UNK && mrks > 0){
                rsnMap[i][j] = what;
                if(rsning(pt)){

                    printf("suppose %c & confict @ yx = %d %d\n",
                            MSSYM[what] , i , j);

                    if(what == FLG)
                        click(pt , LEFT) ,
                            analyAftClk(pt , LEFT);
                    else if(what == SAF)
                        click(pt , RIGHT) ,
                            dirSng(pt);

                    //while(direct());
                }
            }
        }

    puts("suppose return\n");
    return rtn;
}

int rsning(POINT pt){//input a space with num arround !

    int ptIdx = 0;
    POINT *adjptr;

    while(adjptr = adjPts(pt , ptIdx++)){

        int adjUnkNum = adjCnt(*adjptr , UNK , rsnMap);
        int adjFlgNum = adjCnt(*adjptr , FLG , rsnMap);
        int ptMrk = PTON(*adjptr , rsnMap);
        if(HASBOMB(rsnMap , *adjptr) && ptMrk == adjFlgNum)
            if(rsnExpn(*adjptr , SAF)) return 1;

        if(HASBOMB(rsnMap , *adjptr)
                && ptMrk == adjFlgNum + adjUnkNum)
            if(rsnExpn(*adjptr , FLG)) return 1;

        if(HASBOMB(rsnMap , *adjptr)
                && adjUnkNum == 0
                && ptMrk != adjFlgNum)
            return 1;

        free(adjptr);
    }
    return 0;
}

int rsnExpn(POINT pt , int type){

    if(type != FLG && type != SAF)
        puts("wrong type @ rsnExpn") , exit(0);

    POINT que[8] , *adjptr;
    int i , quelen = 0 , ptIdx = 0;
    while(adjptr = adjPts(pt , ptIdx++)){
        if(rsnMap[adjptr->y][adjptr->x] == 0){
            rsnMap[adjptr->y][adjptr->x] = type;
            que[i].y = adjptr->y;
            que[i].x = adjptr->x;
            quelen++;
        }
        free(adjptr);
    }

    for(i = 0 ; i < quelen ; i++)
        if(rsning(que[i])) return 1;

    return 0;
}

#define YONREGION(y) (y == 0 || y == MAPHI - 1)
#define XONREGION(x) (x == 0 || x == MAPWD - 1)
int adjPtNum(POINT pt){
    int num[] = { 8 , 5 , 3 };
    return num[XONREGION(pt.x) + YONREGION(pt.y)];
}

int cntMap(int what, int mat[][MAPWD]) {
    int i , j, rtn = 0;
    if(!(what <= ERR && what >= UNK))
        puts("cntMap Error") , exit(0);

    for (i = 0; i < MAPHI; i++)
        for (j = 0; j < MAPWD; j++)
            rtn += mat[i][j] == what;
    return rtn;
}

int adjCnt(POINT pt , int type , int mat[][MAPWD]) {
    POINT *adjptr;
    int rtn = 0 , ptIdx = 0;
    while(adjptr = adjPts(pt , ptIdx++)){
        if(mat[adjptr->y][adjptr->x] == type)
            rtn++;
        free(adjptr);
    }
    return rtn;
}

int adjCntMrk(POINT pt , int mat[][MAPWD]){
    POINT *adjptr;
    int rtn = 0 , ptIdx = 0;
    while(adjptr = adjPts(pt , ptIdx++)){
        if(HASBOMB(mat , *adjptr))
            rtn++;
        free(adjptr);
    }
    return rtn;
}
