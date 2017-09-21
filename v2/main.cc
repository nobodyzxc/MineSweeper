#include<stdio.h>
#include<windows.h>
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

#define EXPECT(x , y) ((x) == (y))

vector<vector<int> > map , rsnMap , exh;
int flags = FLAGNUM;
int flag_s = 0 , flag_p = 0 , flag_r = 0 ,
    flag_b = 0 , flag_c = 0 , locYX = 0 ,
    relocY = -1 , relocX = -1;

#define EQS(a , b) (!strcmp(a , b))
#define SWAP(a , b) \
    do{ \
        int ____ = a; \
        a = b , b = ____; \
    }while(0)

void parse_flags(int argc , char *argv[]){
    if(argc > 0){
        for(int i = 1 ; i < argc ; i++){
            if(EQS(argv[i] , "-h"))
                printf("%s %s" ,
                        basename(argv[0]) ,
                        "[-sbrp] "
                        "[-xy X_POS Y_POS] "
                        "[-yx Y_POS X_POS]\n\n"
                        "-s  scan the map only\n"
                        "-p  enum every possibility\n"
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
                for(int j = 1 ; j < strlen(argv[i]) ; j++)
                    if(argv[i][j] == 's')
                        flag_s = 1;
                    else if(argv[i][j] == 'b')
                        flag_b = 1;
                    else if(argv[i][j] == 'r')
                        flag_r = 1;
                    else if(argv[i][j] == 'p')
                        flag_p = 1;
                    else if(argv[i][j] == 'c')
                        flag_c = 1;
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

            int _ = 0;
            _ += sscanf(argv[locYX + 1] ,
                    "%d" , &relocY);
            if(_ != 1) puts("parse YX failed") , exit(0);
            _ += sscanf(argv[locYX + 2] ,
                    "%d" , &relocX);
            if(_ != 2) puts("parse YX failed") , exit(0);
        }

    }
}

int main(int argc , char *argv[]){
    system("clear");

    parse_flags(argc , argv);
    srand(time(NULL));

    /* api init */
    setFormLoc(relocY , relocX);

    if(!flag_b) showGamePane();
    else Dputs("back");

    setFormLoc(relocY , relocX); // <<

    initMouse();
    /* init end*/

    init_map();

    if(flag_r || flag_p || flag_s){
        analyMap(true);
        flags = FLAGNUM - cnt_map(FLG , map);
        if(flag_p){
            improved_print(map , flags);
            exit(0);
        }
        else if(flag_s){
            printTab(map , false);
            exit(0);
        }
    }
    else{
        POINT pt = {-1 , -1};
        if(not flag_c)
            clickLeft(pt);        //reset status
        pt.x = MAPWD / 2  , pt.y = MAPHI / 2; //open center
        fancyClickLeft(pt);
        flags = FLAGNUM - cnt_map(FLG , map);
    }

#ifdef DEBUG
    return EXPECT(play() , LOS);
#else
    play();
    printf("flags left:%d , block left:%d\n" ,
            flags , cnt_map(UNK , map));
    printf("\033[40A\033[34B");

    return 0;
#endif
}
