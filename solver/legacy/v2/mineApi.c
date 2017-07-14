#include<windows.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdbool.h>

#include"game.h"

#define BLKWD 16
#define HFBLK (BLKWD/2)
#define MAP_O_FIX_X 18
#define MAP_O_FIX_Y 83
#define PTINMAP(p) ((p).x < MAPWD && (p).x >= 0 \
                        && (p).y < MAPHI && (p).y >= 0)

#define HASBOMB(mp , p) ((mp)[(p).y][(p).x] >= ONE \
                        && (mp)[(p).y][(p).x] <= EAT)

#define PTON(p , mp) ((mp)[(p).y][(p).x])
#define UNOPEN(mp , p) ((mp)[(p).y][(p).x] == UNK \
                        || ((mp)[(p).y][(p).x]) == FLG)
#define OPENED(mp , p) (!UNOPEN(mp , p))

#define CellCenX(px) (ZERO.x + (px) * BLKWD + HFBLK)
#define CellCenY(py) (ZERO.y + (py) * BLKWD + HFBLK)
#define CellLocX(px) (ZERO.x + (px) * BLKWD)
#define CellLocY(py) (ZERO.y + (py) * BLKWD)
#define CellIdtX(px) (ZERO.x + (px) * BLKWD + 9)
#define CellIdtY(py) (ZERO.y + (py) * BLKWD + 4)
#define FORMLOCX (ZERO.x - MAP_O_FIX_X)
#define FORMLOCY (ZERO.y - MAP_O_FIX_Y)
#define WINLOCX (FORMLOCX + 398)
#define WINLOCY (FORMLOCY + 71)
#define SUNGLAS RGB(0 , 0 , 0)
#define LOSLOCX (FORMLOCX + 252)
#define LOSLOCY (FORMLOCY + 56)
#define DEDFACE RGB(0 , 0 , 0)

#define UNK_RGB RGB(189 , 189 , 189)
#define ONE_RGB RGB(0 , 0 , 255)
#define TWO_RGB RGB(0 , 123 , 0)
#define THR_RGB RGB(255 , 0 , 0)
#define FOR_RGB RGB(0 , 0 , 123)
#define FIV_RGB RGB(123 , 0 , 0)
#define SIX_RGB RGB(0 , 123 , 123)
#define SVN_RGB RGB(0 , 0 , 0)
#define EAT_RGB RGB(123 , 123 , 123)
#define FLG_RGB RGB(0 , 0 , 0)//@(y,x)=(11,4)
#define SAF_RGB RGB(189 , 189 , 189)//@(y,x)=(1,1)

#define showGamePane() \
    ShowWindow(GameHwnd , 2); \
    ShowWindow(GameHwnd , SW_SHOWNORMAL); \
    usleep(750000);

enum {
    UNK , ONE , TWO , THR , FOR , FIV ,
    SIX , SVN , EAT , FLG , SAF , ERR
};
const COLORREF CLRNUM[] = {
    [UNK] = UNK_RGB ,
    [ONE] = ONE_RGB ,
    [TWO] = TWO_RGB ,
    [THR] = THR_RGB ,
    [FOR] = FOR_RGB ,
    [FIV] = FIV_RGB ,
    [SIX] = SIX_RGB ,
    [SVN] = SVN_RGB ,
    [EAT] = EAT_RGB ,
    [FLG] = FLG_RGB ,
    [SAF] = SAF_RGB ,
};
const char MSSYM[] = {
    [UNK] = '.' ,//UNK + '0',
    [ONE] = ONE + '0',
    [TWO] = TWO + '0',
    [THR] = THR + '0',
    [FOR] = FOR + '0',
    [FIV] = FIV + '0',
    [SIX] = SIX + '0',
    [SVN] = SVN + '0',
    [EAT] = EAT + '0',
    [FLG] = 'F' ,
    [SAF] = '0' ,
    [ERR] = 'X'
};

POINT ZERO;
HWND GameHwnd;

#include "winerr.c"

#define setFormLoc() \
    HWND MSpane; \
LPRECT MSrect; \
GameHwnd = MSpane = FindWindow(NULL , "Minesweeper"); \
if(!MSpane) puts("no such win") , exit(0); \
if(!GetWindowRect(MSpane , MSrect)){ \
    puts("rect ERR") , printf("%d\n" , MSrect) , exit(0); \
} \
ZERO.x = MSrect->left + MAP_O_FIX_X; \
ZERO.y = MSrect->top + MAP_O_FIX_Y; \
printf("ZERO(x,y)@(%d,%d)\n" , ZERO.x , ZERO.y);


int getColor(COLORREF _color){
    int i;
    for(i = 0 ;
            i < sizeof(CLRNUM) / sizeof(COLORREF) ;
            i++)
        if(_color == CLRNUM[i]) break;
    return i;
}

#define WIN 1
#define RUN 0
#define LOS -1
int gameState(void) {
    HDC _hdc = GetDC(NULL);
    int rtn = RUN;
    if(GetPixel(_hdc , WINLOCX , WINLOCY)
            == SUNGLAS)
        rtn = WIN , puts("win");
    else if(GetPixel(_hdc , LOSLOCX , LOSLOCY)
            == DEDFACE)
        rtn = LOS , puts("lose");
    ReleaseDC(NULL , _hdc);
    return rtn;
}


void printTab(int tab[][MAPWD]){
    int i , j;
    for(i = 0 ; i < MAPHI ; i++){
        for(j = 0 ; j < MAPWD ; j++){
            printf("%c " , MSSYM[tab[i][j]]);
        }
        puts("");
    }
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
}
#include "analyscr.c"
#include "mouse.c"
