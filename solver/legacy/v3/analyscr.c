#include<stdio.h>
#include<unistd.h>

#include"game.h"
#include"mark.h"
#include"msapi.h"
#include"analyscr.h"

//#include "winerr.c"

extern int map[MAPHI][MAPWD] , exh[MAPHI][MAPWD];

static LONG lWidth , lHeight;

POINT ZERO;
HWND GameHwnd = NULL;

int CellCenX(int px){
    return (ZERO.x + (px) * BLKWD + HFBLK);
}

int CellCenY(int py){
    return (ZERO.y + (py) * BLKWD + HFBLK);
}

int CellLocX(int px){
    return (ZERO.x + (px) * BLKWD);
}

int CellLocY(int py){
    return (ZERO.y + (py) * BLKWD);
}

int CellIdtX(int px){
    return (ZERO.x + (px) * BLKWD + 9);
}

int CellIdtY(int py){
    return (ZERO.y + (py) * BLKWD + 4);
}

int FORMLOCX(){
    return (ZERO.x - MAP_O_FIX_X);
}

int FORMLOCY(){
    return (ZERO.y - MAP_O_FIX_Y);
}

void getGameWin(){
    GameHwnd = FindWindow(NULL , "Minesweeper");
}

void setFormLoc(){

    if(!GameHwnd) getGameWin();

    RECT Msrc = { 0 };
    HWND MSpane = GetWindow(GameHwnd , 5);

    if(!MSpane) puts("no such win") , exit(0);
    if(!GetWindowRect(MSpane , &Msrc)){
        //ErrorExit(NULL);
        puts("rect ERR");
        printf("%d\n" , Msrc);
        exit(0);
    }
    ZERO.x = Msrc.left + MAP_O_FIX_X;
    ZERO.y = Msrc.top + MAP_O_FIX_Y;
    //printf("ZERO(x,y)@(%d,%d)\n" , ZERO.x , ZERO.y);
}

COLORREF GetPx(int x , int y , bool update){
    return RGB_TO_RGB(
            GetBMptr(update)[y * lWidth + x]);
}


int analySpt(POINT pt , bool update){

    if(PTON(pt , map) != UNK)
        return PTON(pt , map);

    COLORREF dclr;

    PTON(pt , map) =
        getColor(
                (dclr = GetPx(CellIdtX(pt.x) ,
                              CellIdtY(pt.y) , update)));

    if(PTON(pt , map) == UNK){
        if(ISSAF(pt , update)){
            PTON(pt , exh) = true;
            PTON(pt , map) = SAF;
        }
        else if(ISFLG(pt , update)){
            PTON(pt , exh) = true;
            PTON(pt , map) = FLG;
        }
    }

    return PTON(pt , map);
}

int analyRecr(POINT pt , int visit[][MAPWD]){

    int idx , rtn = 0;
    POINT **adjptr = adjPts(pt);
    for(idx = 0 ; idx < 8 && adjptr[idx] ; idx++){
        if(PTON(*adjptr[idx] , visit))
            continue;
        PTON(*adjptr[idx] , visit) = true;
        if(PTON(*adjptr[idx] , map) == UNK)
            analySpt(*adjptr[idx] , false);

        if(PTON(*adjptr[idx] , map) == SAF)
            analyRecr(*adjptr[idx] , visit);
    }
    for(idx = 0 ; idx < 8 ; idx++)
        free(adjptr[idx]);
    free(adjptr);
    return rtn;
}

LPDWORD GetBMptr(bool update){

    static LPDWORD lpBits;
    static HBITMAP hbmMem;
    static HBITMAP hbmOld;
    static HDC hdcMem;

    if(!update && !lpBits) puts("fuck!BMptr") , exit(0);
    if(!update) return lpBits;


    puts("update map(GetBMptr)");

    static DWORD dwStart;
    dwStart = GetTickCount();
    static HDC hdcScreen;
    hdcScreen = GetDC(HWND_DESKTOP);

    lWidth   = GetDeviceCaps(hdcScreen, HORZRES);
    lHeight  = GetDeviceCaps(hdcScreen, VERTRES);

    static BITMAPINFO bi;
    ZeroMemory(&bi, sizeof(bi));
    bi.bmiHeader.biSize        = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth       = lWidth;
    bi.bmiHeader.biHeight      = -lHeight;
    bi.bmiHeader.biPlanes      = 1;
    bi.bmiHeader.biBitCount    = 32;
    bi.bmiHeader.biCompression = BI_RGB;

    if(hbmMem){
        puts("REALSE!A LOT IN BMptr");
        SelectObject(hdcMem, hbmOld);
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
        //free hdcMem
    }
    //free hbmMem
    //    if(lpBits){
    //        printf("free %p\n" , lpBits);
    //        exit(0);
    //        free(lpBits);
    //    }
    //needn't to free lpBits , system auto
    hbmMem = CreateDIBSection(hdcScreen,
            &bi, DIB_RGB_COLORS, (LPVOID*)&lpBits, NULL, 0);


    if(!lpBits) puts("FUCK!MALLOC FAILD!");
    if(!hbmMem) puts("YES , IT SURELY DIE");
    if(!hbmMem && !lpBits) exit(0);

    if(hbmMem){

        hdcMem = CreateCompatibleDC(NULL);
        hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
        BitBlt(hdcMem, 0, 0,
                lWidth, lHeight, hdcScreen, 0, 0, SRCCOPY);

#define LFT 0
#define RHT lWidth
#define TOP 0
#define BTM lHeight

        BitBlt(NULL , LFT, TOP,
                RHT - LFT,
                BTM - TOP,
                hdcMem, LFT, TOP, SRCCOPY);

    }
    ReleaseDC(HWND_DESKTOP, hdcScreen);

    return lpBits;
}

void analyMap(bool update){
    int y , x;
    GetBMptr(update);
    for(y = 0 ; y < MAPHI ; y++){
        for(x = 0 ; x < MAPWD ; x++){
            POINT pt = {x , y};
            analySpt(pt , false);
        }
    }
}

int gameState(void){
/* GetBMptr(true); */
    int rtn = RUN;
    if(GetPx(WINLOCX , WINLOCY , false)
            == SUNGLAS)
        rtn = WIN , puts("win");
    else if(GetPx(LOSLOCX , LOSLOCY , false)
            == DEDFACE)
        rtn = LOS , puts("lose");
    return rtn;
}
