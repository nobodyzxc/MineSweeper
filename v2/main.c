//#define printTab printWithExh

#include<stdio.h>
#include<windows.h>
#include<Winuser.h>
#include<Windows.h>
#include<process.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>

#include<time.h>
#include<unistd.h>

#include<memory.h>

#include "msapi.h"

#define EXPECT(x , y) ((x) == (y))

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

//#include "main.h"
void initMap();
int Gaming(void);
int direct();
int dirSng(POINT);
void addFlgArd(POINT);
void cpMaptoRsnMap();
int suppose(int);
int rsning(POINT);
int rsnExpn(POINT , int);
int cntMap(int , int matrix[][MAPWD]);
int adjCnt(POINT , int , int mata[][MAPWD]);
int adjCntMrk(POINT pt , int mat[][MAPWD]);
void printWithExh(int tab[][MAPWD]);
void analyAftClk(POINT pt , int);

int need_flag(void);
bool legalRsn(POINT argpt);
void print_map(const char *matrix_name);
int resuppose(void);

int dfs_rsning(void);
void dfs(POINT);
void gessClk(void);
void initMouse(void);
struct notation C(int P , int C);
void addC(struct notation *beadd , struct notation add);
void dfs(POINT);
void gessClk(void);


int main(void){

    srand(time(NULL));
    initApi();
    initMap();

    POINT pt = {-1 , -1};
    click(pt , LEFT);

    pt.x = 15 , pt.y = 8;
    click(pt , LEFT);
    analyAftClk(pt , LEFT);
    printTab(map);
    flags = (float)(FLAGNUM - cntMap(FLG , map));
#ifdef DEBUG
    return EXPECT(Gaming() , LOS);
#else
    Gaming();
    printf("flags left:%d , block left:%d\n"
            , int(flags) , cntMap(UNK , map));
    return 0;
#endif
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


int Gaming(void){
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
        if(gameState() != RUN) return gameState();

        if(suppose(FLG)) continue;

        if(suppose(SAF)) continue;

        if(gameState() != RUN) return gameState();

        if(cntMap(UNK , map) == 0){
            puts("all UNK opened");
            break;
        }

        //resuppose();

        puts("game ooo");
        printWithExh(map);
        puts("should exit but try dir again");
        if(direct()) continue;
        puts("");
        printWithExh(map);
        return gameState();

    }
    puts("break");
    return gameState();
}

int direct(){
    puts("map:");
    printTab(map);
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
    POINT **adjptr = adjPts(pt);
    int idx = 0;
    for(idx = 0 ; idx < 8 && adjptr[idx] ; idx++){
        if(PTON(*adjptr[idx] , map) == UNK){
            click(*adjptr[idx] , RIGHT);
            PTON(*adjptr[idx] , map) = FLG;
            flags--;
        }
    }
    for(idx = 0 ; idx < 8 ; idx++)
        free(adjptr[idx]);
    free(adjptr);
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

                    printWithExh(map);

                    if(what == FLG){
                        click(pt , LEFT);
                        analyAftClk(pt , LEFT);
                    }
                    else if(what == SAF){
                        click(pt , RIGHT);
                        PTON(pt , map) = FLG;
                        flags--;
                        dirSng(pt);
                    }
                    //while(direct());
                }
            }
        }

    puts("suppose return\n");
    return rtn;
}

int rsning(POINT pt){//input a space with num arround !

    int idx = 0;
    POINT **adjptr = adjPts(pt);
    for(idx = 0 ; idx < 8 && adjptr[idx] ; idx++){

        puts("fuck here");

        printf("adj*Num (%d,%d)\n" , adjptr[idx]->y ,
                adjptr[idx]->x);

        int adjUnkNum = adjCnt(*adjptr[idx] , UNK , rsnMap);
        int adjFlgNum = adjCnt(*adjptr[idx] , FLG , rsnMap);

        if(!PTINMAP(*adjptr[idx])){
            puts("not in map");
            exit(0);
        }
        int ptMrk = PTON(*adjptr[idx] , rsnMap);
        if(HASBOMB(rsnMap , *adjptr[idx]) && ptMrk == adjFlgNum){

            printTab(rsnMap);
            printf("expn 0 (%d,%d)\n" , adjptr[idx]->y ,
                    adjptr[idx]->x);
            if(rsnExpn(*adjptr[idx] , SAF)) return 1;
        }

        if(HASBOMB(rsnMap , *adjptr[idx])
                && ptMrk == adjFlgNum + adjUnkNum){

            printTab(rsnMap);
            printf("expn F (%d,%d)\n" , adjptr[idx]->y ,
                    adjptr[idx]->x);
            if(rsnExpn(*adjptr[idx] , FLG)) return 1;
        }

        if(HASBOMB(rsnMap , *adjptr[idx])
                && adjUnkNum == 0
                && ptMrk != adjFlgNum){
            printTab(rsnMap);
            printf("@(%d,%d) conflict\n"
                    "HASbomb %d\n"
                    "adjUnkum %d\n"
                    "ptMrk -%d-\n"
                    "adjFlgNum %d\n" ,
                    adjptr[idx]->y ,
                    adjptr[idx]->x ,
                    HASBOMB(rsnMap , *adjptr[idx]) ,
                    adjUnkNum ,
                    ptMrk ,
                    adjFlgNum);
            puts("again fuck here");
            adjCnt(*adjptr[idx] , FLG , rsnMap);
            adjCnt(*adjptr[idx] , UNK , rsnMap);
            return 1;
        }
    }
    for(idx = 0 ; idx < 8 ; idx++)
        free(adjptr[idx]);
    free(adjptr);

    return 0;
}

int rsnExpn(POINT pt , int type){

    if(type != FLG && type != SAF)
        puts("wrong type @ rsnExpn") , exit(0);

    POINT que[8] , **adjptr = adjPts(pt);
    int quelen = 0 , idx;
    for(idx = 0 ; idx < 8 && adjptr[idx] ; idx++){
        if(PTON(*adjptr[idx] , rsnMap) == UNK){
            PTON(*adjptr[idx] , rsnMap) = type;
            que[idx].y = adjptr[idx]->y;
            que[idx].x = adjptr[idx]->x;
            quelen++;
        }
    }
    for(idx = 0 ; idx < 8 ; idx++)
        free(adjptr[idx]);
    free(adjptr);

    printTab(rsnMap);

    for(idx = 0 ; idx < quelen ; idx++)
        if(rsning(que[idx])) return 1;

    return 0;
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
    int rtn = 0 , idx = 0;
    POINT **adjptr = adjPts(pt);
    for(idx = 0 ; idx < 8 && adjptr[idx] ; idx++){
        if(PTON(*adjptr[idx] , mat) == type)
            rtn++;
    }
    for(idx = 0 ; idx < 8 ; idx++)
        free(adjptr[idx]);
    free(adjptr);
    return rtn;
}

int adjCntMrk(POINT pt , int mat[][MAPWD]){
    int rtn = 0 , idx = 0;
    POINT **adjptr = adjPts(pt);
    for(idx = 0 ; idx < 8 && adjptr[idx] ; idx++){
        if(HASBOMB(mat , *adjptr[idx]))
            rtn++;
    }
    for(idx = 0 ; idx < 8 ; idx++)
        free(adjptr[idx]);
    free(adjptr);
    return rtn;
}

void printWithExh(int tab[][MAPWD]){
    char fmt[2][30] = {
        "%c " ,
        "\033[1;33m%c\033[0m "
    };
    int i , j;
    for(i = 0 ; i < MAPHI ; i++){
        for(j = 0 ; j < MAPWD ; j++){
            printf(fmt[exh[i][j]] , MSSYM[tab[i][j]]);
        }
        puts("");
    }
    //fflush(stdout);
    return;
}

void analyAftClk(POINT pt , int type){

    //usleep(500000);
    w_usleep(50000);
    //wait result after click

    if(!PTINMAP(pt))
        puts("error analy pos") , exit(0);

    int visit[MAPHI][MAPWD];
    memset(visit, 0, sizeof(visit));
    if(type == BOTH){
        GetBMptr(true);//um... I forget it.
        PTON(pt , visit) = true;
        analyRecr(pt , visit);
        direct();
    }
    else if(type == LEFT){
        GetBMptr(true);
        analySpt(pt , false);
        if(HASBOMB(map , pt)){
            dirSng(pt);
        }
        else if(map[pt.y][pt.x] == SAF){
            PTON(pt , visit) = true;
            analyRecr(pt , visit);
            puts("ANALY OF LEFT CLK @ SAF DONE");
            direct();
        }
    }
    else if(type == RIGHT){
        int idx = 0;
        POINT **adjptr = adjPts(pt);
        if(!adjPts) puts("adjPts error");
        for(idx = 0 ; idx < 8 && adjptr[idx] ; idx++){
            dirSng(*adjptr[idx]);
        }
        for(idx = 0 ; idx < 8 ; idx++)
            free(adjptr[idx]);
        free(adjptr);
    }
}

#include "dirty.c"
