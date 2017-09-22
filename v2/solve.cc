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
#include "point.h"
#include "enumerate.h"
#include "solve.h"

#include<vector>
using namespace std;

extern int flags;
extern vector<vector<int> > map , rsnMap , exh;

int cnt_adj_mrk(POINT pt , vector<vector<int> > mat){
    int rtn = 0;
    RPTP(mat , pt , p , {
        if(HASBOMB(mat , p)) rtn++;
    });
    return rtn;
}

void init_map(){
    RPTHI(i){
        map.push_back(vector<int>(MAPWD , 0));
        exh = rsnMap = map;
    }
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

int dirSng(POINT pt){
    int modify = 0;
    if(!exh[pt.y][pt.x] and HASBOMB(map , pt)){
        int adjUnkNum = cnt_adj(pt , UNK , map);
        int adjFlgNum = cnt_adj(pt , FLG , map);
        if(adjUnkNum == 0
                && PTON(pt , map) == adjFlgNum){
            Dprintf("(%d %d) exh\n" , pt.y , pt.x);
            PTON(pt , exh) = true;
        }
        else if(adjFlgNum == map[pt.y][pt.x]){
            modify = 1;
            Dprintf("(%d %d) expand\n" , pt.y , pt.x);
            PTON(pt , exh) = true;
            fancyClickBoth(pt);
        }
        else if(adjUnkNum ==
                map[pt.y][pt.x] - adjFlgNum){
            modify = 1;
            Dprintf("(%d %d) flgard\n" , pt.y , pt.x);
            PTON(pt , exh) = true;
            addFlgArd(pt);
        }
    }
    return modify;
}

int direct(){
    Dputs("dir beg");
    int modify = 0;
    RPTMAP(y , x){
        POINT pt = {x , y};
        modify |= dirSng(pt);
    }
    Dputs("dir end");
    return modify;
}

void analyAftClk(POINT pt , int type){
    bool change = false;

    Dputs("analy...aft click");

    //w_usleep(1000);//50000
    //wait result after click

    if(!PTINVEC(pt , map))
        puts("error analy pos") , exit(0);

    vector<vector<int> > visit;
    visit = exh;
    //RPTHI(i)
    //    visit.push_back(vector<int>(MAPWD , 0));
    if(type == BOTH){
        GetBMptr(true);//um... I forget it.
        Dputs("bp on 196");
        PTON(pt , visit) = true;
        Dputs("start analyRecr");
        change |= analyRecr(pt , visit);
        Dputs("after analyRecr");
        Dputs("start direct");
        change |= direct();
        Dputs("after direct");
    }
    else if(type == LEFT){
        GetBMptr(true);
        Dputs("bp on 203");
        change |= analySpt(pt , false) != UNK;
        Dputs("analing spot");
        if(HASBOMB(map , pt)){
            change |= dirSng(pt);
        }
        else if(map[pt.y][pt.x] == SAF){
            PTON(pt , visit) = true;
            Dputs("recr");
            change |= analyRecr(pt , visit);
            Dputs("ANALY OF LEFT CLK @ SAF DONE");
            change |= direct();
        }
    }
    else if(type == RIGHT){
        RPTP(map , pt , p , {
            change |= dirSng(p);
            });
    }
    Dputs("analy...aft click end");
    if(gameState() != RUN) exit(0);
    if(not change) puts("re") , analyAftClk(pt , type);
}

void fancyClickLeft(POINT p){
    clickLeft(p);
    analyAftClk(p , LEFT);
}

void fancyClickBoth(POINT p){
    clickBoth(p);
    analyAftClk(p , BOTH);
}

int rsning(POINT pt){//input a space with num arround !

    RPTP(map , pt , p , {

        Dprintf("adj*Num (%d,%d)\n" , p.y ,
                p.x);

        int adjUnkNum = cnt_adj(p , UNK , rsnMap);
        int adjFlgNum = cnt_adj(p , FLG , rsnMap);

        if(!PTINVEC(p , map)){
            puts("not in map");
            exit(0);
        }
        int ptMrk = PTON(p , rsnMap);
        if(HASBOMB(rsnMap , p) && ptMrk == adjFlgNum){

            Dprintf("expn 0 (%d,%d)\n" , p.y , p.x);
            if(rsnExpn(p , SAF)) return 1;
        }

        if(HASBOMB(rsnMap , p)
                && ptMrk == adjFlgNum + adjUnkNum){

            Dprintf("expn F (%d,%d)\n" , p.y , p.x);
            if(rsnExpn(p , FLG)) return 1;
        }

        if(HASBOMB(rsnMap , p)
                && adjUnkNum == 0
                && ptMrk != adjFlgNum){
            Dprintf("@(%d,%d) conflict\n"
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

int suppose(int what){
    int rtn = 0;

    Dputs("suppose begin");

    RPTMAP(i , j){
        rsnMap = map;
        rsnMap = map;
        POINT pt = {j , i};
        int mrks = cnt_adj_mrk(pt , map);
        if(PTON(pt , map) == UNK && mrks > 0){
            rsnMap[i][j] = what;
            if(rsning(pt)){
                rtn = 1;
                Dprintf("suppose %c & confict "
                        "@ yx = %d %d\n",
                        MSSYM[what] , i , j);
                if(what == FLG){
                    fancyClickLeft(pt);
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

    Dputs("suppose return\n");
    return rtn;
}

int play(){

    while(1){
        Dputs("solving");
        if(cnt_map(UNK , map) == MAPHI * MAPWD){
            Dputs("guess");
            exit(0);
            analyMap(true);
            continue;
        }
        Dputs("directing");
        if(direct()) continue;

        if(gameState() != RUN) return gameState();

        Dputs("supposing FLG");
        if(suppose(FLG)) continue;

        Dputs("supposing SAF");
        if(suppose(SAF)) continue;

        if(gameState() != RUN) return gameState();

        if(cnt_map(UNK , map) == 0){
            Dputs("all UNK opened");
            break;
        }

        Dputs("improved solving");
        if(improved_sol(map , flags , fancyClickLeft , clickRight)){
            if(gameState() != RUN) return gameState();
            continue;
        }
        return gameState();
    }
    Dputs("break");
    return gameState();
}
