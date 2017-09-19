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

#include<vector>
using namespace std;

typedef unsigned int uint;
typedef vector<vector<int> > vvint;

#define EXPECT(x , y) ((x) == (y))

vvint map , rsnMap , exh;
int flags = FLAGNUM;

void init_map();
int play(void);
int direct();
int dirSng(POINT);
void addFlgArd(POINT);
int suppose(int);
int rsning(POINT);
int rsnExpn(POINT , int);
void analyAftClk(POINT pt , int);
void parse_flags(int argc , char *argv[]);

#define EQS(a , b) (!strcmp(a , b))
#define SWAP(a , b) \
    do{ \
        int ____ = a; \
        a = b , b = ____; \
    }while(0)

int scan = 0 , locYX = 0 , hide = 0 ,
    relocY = -1 , relocX = -1 , relay = 0;

void init_map(){
    RPTHI(i){
        map.push_back(vector<int>(MAPWD , 0));
        exh = rsnMap = map;
    }
}

void printTab(vvint tab){
    char fmt[2][30] = {
        "%c " , "\033[1;33m%c\033[0m "
    };
    NRPT2V(tab , i , j , {
        printf(fmt[exh[i][j]] , MSSYM[tab[i][j]]);
        } , { puts(""); });
    return;
}

int main(int argc , char *argv[]){

    parse_flags(argc , argv);

    srand(time(NULL));

    /* api init */
    checkResolution();

    if(locYX)
        setZEROLoc(relocY , relocX);
    else
        setFormLoc();

    if(!hide)
        showGamePane();
    else puts("back");

    initMouse();
    /* init end*/

    init_map();

    if(relay){
        analyMap(true);
        if(scan) printTab(map) , exit(0);
    }
    else{
        POINT pt = {-1 , -1};
        clickLeft(pt);

        pt.x = 15 , pt.y = 8; //open center

        clickLeft(pt);
        analyAftClk(pt , LEFT);
    }

    flags = FLAGNUM - cnt_map(FLG , map);
#ifdef DEBUG
    return EXPECT(play() , LOS);
#else
    play();
    printf("flags left:%d , block left:%d\n" ,
            flags , cnt_map(UNK , map));
    return 0;
#endif
}

void parse_flags(int argc , char *argv[]){
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
                printf("unknown parameter %s\n" ,
                        argv[i]) , exit(0);
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

int play(void){

    puts("gaming");

    while(1){

        if(cnt_map(UNK , map) == MAPHI * MAPWD){
            puts("guess");
            exit(0);
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

        if(cnt_map(UNK , map) == 0){
            puts("all UNK opened");
            break;
        }

        //resuppose();

        puts("game ooo");
        printTab(map);
        puts("should exit but try dir again");
        if(direct()) continue;
        puts("");
        printTab(map);
        return gameState();

    }
    puts("break");
    return gameState();
}

int direct(){
    puts("dir beg");
    int modify = 0;
    RPTMAP(y , x){
        POINT pt = {x , y};

        if(dirSng(pt)){
            printf("(%d %d) let modify\n" ,
                    pt.y , pt.x);
            modify = 1;
        }
    }
    puts("dir end");
    return modify;
}

int dirSng(POINT pt){
    int modify = 0;
    if(!exh[pt.y][pt.x] and HASBOMB(map , pt)){
        int adjUnkNum = cnt_adj(pt , UNK , map);
        int adjFlgNum = cnt_adj(pt , FLG , map);
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
        else if(adjUnkNum ==
                map[pt.y][pt.x] - adjFlgNum){
            printf("(%d %d) flgard\n" , pt.y , pt.x);
            modify = 1;
            addFlgArd(pt);
            PTON(pt , exh) = true;
        }
    }
    return modify;
}

void addFlgArd(POINT pt){
    RPTP(map , pt , p , {
        if(PTON(p , map) == UNK){
            clickRight(p);
            PTON(p , map) = FLG;
            flags--;
        }
    });
    return ;
}

int suppose(int what){
    int rtn = 0;

    puts("suppose begin");

    RPTMAP(i , j){
        rsnMap = map;
        rsnMap = map;
        POINT pt = {j , i};
        int mrks = cnt_adj_mrk(pt , map);
        if(PTON(pt , map) == UNK && mrks > 0){
            rsnMap[i][j] = what;
            if(rsning(pt)){
                rtn = 1;
                printf("suppose %c & confict "
                        "@ yx = %d %d\n",
                        MSSYM[what] , i , j);
                printTab(map);
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
            }
        }
    }

    puts("suppose return\n");
    return rtn;
}

int rsning(POINT pt){//input a space with num arround !

    RPTP(map , pt , p , {

        printf("adj*Num (%d,%d)\n" , p.y ,
                p.x);

        int adjUnkNum = cnt_adj(p , UNK , rsnMap);
        int adjFlgNum = cnt_adj(p , FLG , rsnMap);

        if(!PTINVEC(p , map)){
            puts("not in map");
            exit(0);
        }
        int ptMrk = PTON(p , rsnMap);
        if(HASBOMB(rsnMap , p) && ptMrk == adjFlgNum){

            printf("expn 0 (%d,%d)\n" , p.y ,
                    p.x);
            if(rsnExpn(p , SAF)) return 1;
        }

        if(HASBOMB(rsnMap , p)
                && ptMrk == adjFlgNum + adjUnkNum){

            printf("expn F (%d,%d)\n" , p.y ,
                    p.x);
            if(rsnExpn(p , FLG)) return 1;
        }

        if(HASBOMB(rsnMap , p)
                && adjUnkNum == 0
                && ptMrk != adjFlgNum){
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
            cnt_adj(p , FLG , rsnMap);
            cnt_adj(p , UNK , rsnMap);
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
    RPTP(map , pt , p , {
        if(PTON(p , rsnMap) == UNK){
            PTON(p , rsnMap) = type;
            que[quelen].y = p.y; //p_idx -> quelen
            que[quelen].x = p.x; // Is it a bug ?
            quelen++;
        }
    });

    for(int idx = 0 ; idx < quelen ; idx++)
        if(rsning(que[idx])) return 1;

    return 0;
}

void analyAftClk(POINT pt , int type){

    w_usleep(50000);
    //wait result after click

    if(!PTINVEC(pt , map))
        puts("error analy pos") , exit(0);

    vvint visit;
    RPTHI(i)
        visit.push_back(vector<int>(MAPWD , 0));

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
        RPTP(map , pt , p , {
            dirSng(p);
            });
    }
}
