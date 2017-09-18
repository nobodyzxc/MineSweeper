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
#include<libgen.h>

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
void parseFlags(int argc , char *argv[]);

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

#define EQS(a , b) (!strcmp(a , b))
#define SWAP(a , b) \
    do{ \
        int ____ = a; \
        a = b , b = ____; \
    }while(0)

int scan = 0 , locYX = 0 , hide = 0 ,
    relocY = -1 , relocX = -1 , relay = 0;

int main(int argc , char *argv[]){

    parseFlags(argc , argv);

    srand(time(NULL));

    {
        checkResolution();

        if(locYX)
            setZEROLoc(relocY , relocX);
        else
            setFormLoc();

        if(!hide)
            showGamePane();
        else puts("back");

        initMouse();
    }

    initMap();

    if(relay){
        analyMap(true);
        if(scan) printWithExh(map) , exit(0);
    }
    else{
        POINT pt = {-1 , -1};
        clickLeft(pt);

        pt.x = 15 , pt.y = 8; //open center

        clickLeft(pt);
        analyAftClk(pt , LEFT);
    }
    printTab(map);
    flags = (float)(FLAGNUM - cntMap(FLG , map));
#ifdef DEBUG
    return EXPECT(Gaming() , LOS);
#else
    Gaming();
    printf("flags left:%d , block left:%d\n" ,
            (int)flags , cntMap(UNK , map));
    return 0;
#endif
}

void parseFlags(int argc , char *argv[]){
    if(argc > 0){
        int i , j;

        for(i = 1 ; i < argc ; i++){
            if(EQS(argv[i] , "-h"))
                printf("%s %s" ,
                        basename(argv[0]) ,
                        "[-sbr] "
                        "[-xy X_POS Y_POS] "
                        "[-yx Y_POS X_POS]\n\n"
                        "-s  scan the map only\n"
                        "-b  solve without "
                        "poping up nw game\n"
                        "-r  relay the game\n"
                        "-xy X_POS Y_POS "
                        "assign X and Y\n"
                        "-yx Y_POS X_POS "
                        "assign Y and X\n") ,
                    exit(0);
            else if(EQS(argv[i] , "-xy"))
                locYX = -i;
            else if(EQS(argv[i] , "-yx"))
                locYX = i;
            else if(argv[i][0] == '-')
                for(j = 1 ; j < strlen(argv[i]) ; j++)
                    if(argv[i][j] == 's')
                        scan = 1 , relay = 1;
                    else if(argv[i][j] == 'b')
                        hide = 1;
                    else if(argv[i][j] == 'r')
                        relay = 1;
                    else{
                        printf("unknown flag %c in %s\n"
                                , argv[i][j] , argv[i]);
                        exit(0);
                    }
            else if((locYX == 0)
                    or (i != abs(locYX) + 1
                        and i != abs(locYX) + 2))
                printf("unknown parameter %s\n" , argv[i]) , exit(0);
        }
        if(locYX){
            if(locYX < 0){
                SWAP(relocX , relocY);
                locYX = -locYX;
            }

            if(argc <= locYX + 2)
                puts("please give your YX") , exit(0);

            int _;
            _ += sscanf(argv[locYX + 1] ,
                    "%d" , &relocY);

            _ += sscanf(argv[locYX + 2] ,
                    "%d" , &relocX);

            if(_ != 2)
                puts("parse YX failed") , exit(0);
        }

    }
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

        if(cntMap(UNK , map) == MAPHI * MAPWD){
            puts("guess");
            exit(0);
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
                clickBoth(pt);
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
    RPTP(pt , p , {
        if(PTON(p , map) == UNK){
            clickRight(p);
            PTON(p , map) = FLG;
            flags--;
        }
    });
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
                        clickLeft(pt);
                        analyAftClk(pt , LEFT);
                    }
                    else if(what == SAF){
                        clickRight(pt);
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


    RPTP(pt , p , {

        printf("adj*Num (%d,%d)\n" , p.y ,
                p.x);

        int adjUnkNum = adjCnt(p , UNK , rsnMap);
        int adjFlgNum = adjCnt(p , FLG , rsnMap);

        if(!PTINMAP(p)){
            puts("not in map");
            exit(0);
        }
        int ptMrk = PTON(p , rsnMap);
        if(HASBOMB(rsnMap , p) && ptMrk == adjFlgNum){

            printTab(rsnMap);
            printf("expn 0 (%d,%d)\n" , p.y ,
                    p.x);
            if(rsnExpn(p , SAF)) return 1;
        }

        if(HASBOMB(rsnMap , p)
                && ptMrk == adjFlgNum + adjUnkNum){

            printTab(rsnMap);
            printf("expn F (%d,%d)\n" , p.y ,
                    p.x);
            if(rsnExpn(p , FLG)) return 1;
        }

        if(HASBOMB(rsnMap , p)
                && adjUnkNum == 0
                && ptMrk != adjFlgNum){
            printTab(rsnMap);
            printf("@(%d,%d) conflict\n"
                    "HASbomb %d\n"
                    "adjUnkum %d\n"
                    "ptMrk -%d-\n"
                    "adjFlgNum %d\n" ,
                    p.y ,
                    p.x ,
                    HASBOMB(rsnMap , p) ,
                    adjUnkNum ,
                    ptMrk ,
                    adjFlgNum);
            puts("again fuck here");
            adjCnt(p , FLG , rsnMap);
            adjCnt(p , UNK , rsnMap);
            return 1;
        }
    });

    return 0;
}

int rsnExpn(POINT pt , int type){

    if(type != FLG && type != SAF)
        puts("wrong type @ rsnExpn") , exit(0);

    int quelen = 0;
    POINT que[8];
    RPTP(pt , p , {
        if(PTON(p , rsnMap) == UNK){
            PTON(p , rsnMap) = type;
            que[quelen].y = p.y; //p_idx -> quelen
            que[quelen].x = p.x; // Is it a bug ?
            quelen++;
        }
    });

    printTab(rsnMap);

    for(int idx = 0 ; idx < quelen ; idx++)
        if(rsning(que[idx])) return 1;

    return 0;
}

int cntMap(int what, int mat[][MAPWD]) {
    int i , j, rtn = 0;
    if(!(what <= ERR && what >= UNK))
        puts("cntMap Error") , exit(0);

    for(i = 0; i < MAPHI; i++)
        for(j = 0; j < MAPWD; j++)
            rtn += mat[i][j] == what;
    return rtn;
}

int adjCnt(POINT pt , int type , int mat[][MAPWD]) {
    int rtn = 0;
    RPTP(pt , p ,
            { if(PTON(p , mat) == type) rtn++; });
    return rtn;
}

int adjCntMrk(POINT pt , int mat[][MAPWD]){
    int rtn = 0;
    RPTP(pt , p , {
        if(HASBOMB(mat , p)) rtn++;
    });
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
        RPTP(pt , p , {
            dirSng(p);
            });
    }
}

#include "dirty.cc"
