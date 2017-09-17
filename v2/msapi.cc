#include<windows.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdbool.h>

#include"msapi.h"

extern HWND GameHwnd;

void initApi(int y , int x){
    checkResolution();
    setFormLoc();
    showGamePane();
    initMouse();
}

extern POINT ZERO;
void showGamePane(){
    ShowWindow(GameHwnd , 2);
    ShowWindow(GameHwnd , SW_SHOWNORMAL);
    w_usleep(750000);
}

int getColor(COLORREF _color){
    int i;
    for(i = 0 ;
            i < sizeof(CLRNUM) / sizeof(COLORREF) ;
            i++)
        if(_color == CLRNUM[i]) break;
    return i;
}

void printTab(int tab[][MAPWD]){
    return;
    int i , j;
    for(i = 0 ; i < MAPHI ; i++){
        for(j = 0 ; j < MAPWD ; j++){
            printf("%c " , MSSYM[tab[i][j]]);
        }
        puts("");
    }
}

POINT **adjPts(POINT pt){
    POINT **rtn = (POINT**)calloc(8 , sizeof(POINT*)) ,
                 d[] = {
                     {0 , -1} , { 1 , -1} ,
                     {1 ,  0} , { 1 ,  1} ,
                     {0 ,  1} , {-1 ,  1} ,
                     {-1 , 0} , {-1 , -1}
                 };
    if(!rtn) puts("calloc failed!") , exit(0);
    int i , idx = 0;
    for(i = 0 ; i < 8 ; i++){
        POINT tmp = {
            pt.x + d[i].x ,
            pt.y + d[i].y
        };

        if(PTINMAP(tmp)){
            rtn[idx] =
                (POINT*)malloc(sizeof(POINT));
            rtn[idx]->x = tmp.x;
            rtn[idx]->y = tmp.y;
            idx++;
        }
    }
    return rtn;
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

    ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
