#include<windows.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdbool.h>

#include"msapi.h"

extern HWND GameHwnd;

void initApi(int y , int x){
    checkResolution();
    setFormLoc(0 , 0);
    showGamePane();
    initMouse();
}

extern POINT ZERO;

void showGamePane(){
    if(!GameHwnd) getGameWin();
    ShowWindow(GameHwnd , 2);
    ShowWindow(GameHwnd , SW_SHOWNORMAL);
    w_usleep(750000);
}

int getColor(COLORREF _color){
    int i;
    for(i = 0 ;
            i < (int)(sizeof(CLRNUM) / sizeof(COLORREF)) ;
            i++)
        if(_color == CLRNUM[i]) break;
    return i;
}

#define YONREGION(y) (y == 0 || y == MAPHI - 1)
#define XONREGION(x) (x == 0 || x == MAPWD - 1)
int adjPtNum(POINT pt){
    int num[] = { 8 , 5 , 3 };
    return num[XONREGION(pt.x) + YONREGION(pt.y)];
}


void w_usleep(__int64 usec)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10*usec);
    // Convert to 100 nanosecond interval,
    // negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

extern vector<vector<int> >exh;
void printTab(vector<vector<int> > tab , bool move){
    if(move)
        printf("\033[40A\033[1B");
    char fmt[2][30] = {
        "%c " , "\033[1;33m%c\033[0m "
    };
    NRPT2V(tab , i , j , {
        printf(fmt[exh[i][j]] , MSSYM[tab[i][j]]);
        } , { puts(""); });
    return;
}
