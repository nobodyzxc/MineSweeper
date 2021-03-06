#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>

#include"point.h"
#include"game.h"
#include"mark.h"
#include"msapi.h"
#include"analyscr.h"

extern vector<vector<int> > map , exh;

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
    if(!GameHwnd)
        puts("Please start your game.") ,
            printTab(map , false) , exit(0);
}

void setFormLoc(int y , int x){
    if(y >= 0 or x >= 0){
        printf("set From to %d %d\n" , y , x);
        ZERO.y = y;
        ZERO.x = x;
    }
    else{
        if(!GameHwnd) getGameWin();

        RECT Msrc = { 0 };
        HWND MSpane = GetWindow(GameHwnd , 5);

        if(!MSpane) puts("Cannot Get Child Window") , exit(0);
        if(!GetWindowRect(MSpane , &Msrc)){
            puts("rect ERR");
            exit(0);
        }
        ZERO.x = Msrc.left + MAP_O_FIX_X;
        ZERO.y = Msrc.top + MAP_O_FIX_Y;
        printf("ZERO.x = %d , ZERO.y = %d\n" ,
                ZERO.x , ZERO.y);
    }
    checkResolution();
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
    else if(PTON(pt , map) == ERR)
        printf("ERR scan on %d %d" , pt.y , pt.x) , exit(1);

    return PTON(pt , map);
}

int analyRecr(POINT pt , vector<vector<int> > &visit){
                        // damn it.
    visit[pt.y][pt.x] = 1;
    bool change = false;
    RPTP(visit , pt , p , {
            if(p.y == pt.y and p.x == pt.x){
                puts("analyscr.cc:124"); exit(0);
            }
            if(PTON(p , visit))
                continue;
            PTON(p , visit) = true;
            if(PTON(p , map) == UNK)
                change |= analySpt(p , false) != UNK;
            if(PTON(p , map) == SAF)
                analyRecr(p , visit);
            });

    return change;
}

LPDWORD GetBMptr(bool update){

    static LPDWORD lpBits;
    static HBITMAP hbmMem;
    static HBITMAP hbmOld;
    static HDC hdcMem;

    if(!update && !lpBits) puts("BMptr should init") , exit(0);
    if(!update) return lpBits;


    Dputs("update map(GetBMptr)");
    w_usleep(30000); // wait for click ...

    GetTickCount();
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
        Dputs("REALSE!A LOT IN BMptr");
        SelectObject(hdcMem, hbmOld);
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
        //free hdcMem , hbmMem
    }
    //needn't to free lpBits , system auto
    hbmMem = CreateDIBSection(hdcScreen,
            &bi, DIB_RGB_COLORS, (LPVOID*)&lpBits, NULL, 0);

    if(!lpBits) puts("MALLOC FAILD!");
    if(!hbmMem) puts("YES , IT SURELY DIE");
    if(!hbmMem && !lpBits) puts("NULL hbmMem") , exit(0);

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
    Dputs("analy screen end");
    return lpBits;
}

void analyMap(bool update){
    GetBMptr(update);
    RPTMAP(y , x){
        POINT pt = {x , y};
        analySpt(pt , false);
    }
}

int gameState(void){
    /* GetBMptr(true); */
    int rtn = RUN;
    if(GetPx(WINLOCX , WINLOCY , false)
            == SUNGLAS)
        rtn = WIN;// , puts("win");
    else if(GetPx(LOSLOCX , LOSLOCY , false)
            == DEDFACE)
        rtn = LOS;// , puts("lose");
    return rtn;
}

void checkResolution(void){

    HDC hdcScreen = GetDC(HWND_DESKTOP);

    int sup[12][2] =
    {
        {1600 , 900} ,
        {1440 , 900} ,
        {1366 , 768} ,
        {1360 , 768} ,
        {1280 , 960} ,
        {1280 , 800} ,
        {1280 , 768} ,
        {1280 , 720} ,
        {1280 , 600} ,
        {1152 , 864} ,
        {1024 , 768} ,
        { 800 , 600}
    } ,
        unsup[4][2] =
        {
            {1920 , 1080} ,
            {1680 , 1050} ,
            {1400 , 1050} ,
            {1280 , 1024} ,
        };

    int w = GetDeviceCaps(hdcScreen, HORZRES) , knw = 0;
    int h = GetDeviceCaps(hdcScreen, VERTRES) , i;

    if(ZERO.y < 0
            || ZERO.y > h
            || ZERO.x < 0
            || ZERO.x > w)
        puts("Invalid form position") , exit(0);

    for(i = 0 ; i < 12 ; i++)
        if(sup[i][0] == w && sup[i][1] == h)
            return;

    for(i = 0 ; i < 4 ; i++)
        if(unsup[i][0] == w && unsup[i][1] == h)
            printf("unsupported resolution : %d x %d \n" , w , h);// , knw = 1;

    puts("resolution may support:");
    for(i = 0 ; i < 12 ; i++)
        printf("\t%d x %d\n" , sup[i][0] , sup[i][1]);

    puts("resolution may not support:");
    for(i = 0 ; i < 4 ; i++)
        printf("\t%d x %d\n" , unsup[i][0] , unsup[i][1]);

    if(!knw){
        puts("I am not sure it will run normally in your resolution.");
        printf("Do you want to continue?(y/n) "); fflush(stdout);
        char res[100]; scanf("%s" , res);
        if(res[0] == 'y' || res[0] == 'Y') return;
    }
    exit(0);
}
