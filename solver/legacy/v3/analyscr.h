#ifndef ANALYSCR_H
#define ANALYSCR_H

#include<windows.h>
#include"mark.h"
#include"game.h"
/* MAPHI MAPWD */

#define BLKWD 16
#define HFBLK (BLKWD/2)
#define MAP_O_FIX_X 10
#define MAP_O_FIX_Y 52

int CellCenX(int px);
int CellCenY(int py);
int CellLocX(int px);
int CellLocY(int py);
int CellIdtX(int px);
int CellIdtY(int py);
int FORMLOCX();
int FORMLOCY();

#define WINLOCX (FORMLOCX() + 244)
#define WINLOCY (FORMLOCY() + 25)
#define SUNGLAS RGB(0 , 0 , 0)
#define LOSLOCX (FORMLOCX() + 246)
#define LOSLOCY (FORMLOCY() + 30)
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

#define RGB_TO_RGB(p) \
    ((((p) & 0xFF) << 16) \
     | ((p) & 0xFF00) \
     | (((p) & 0xFF0000) >> 16) )

#define ISSAF(p , update) \
    (dclr = GetPx(CellLocX((p).x) + 1 , \
                  CellLocY((p).y) + 1 , update)) == SAF_RGB

#define  ISFLG(p , update) \
    (dclr = GetPx(CellLocX((p).x) + 4 , \
                  CellLocY((p).y) + 11 , update)) == FLG_RGB

#define printClr(clr) \
{ int r = GetRValue(clr) , \
    g = GetGValue(clr) , \
    b = GetBValue(clr);\
    printf("RGB(%d , %d , %d)\n" , r , g , b); \
}

void getGameWin();

void setFormLoc();

COLORREF GetPx(int x , int y , bool update);

int analySpt(POINT pt , bool update);

int analyRecr(POINT pt , int visit[][MAPWD]);

void analyMap(bool update);

LPDWORD GetBMptr(bool update);

int gameState(void);
#endif
