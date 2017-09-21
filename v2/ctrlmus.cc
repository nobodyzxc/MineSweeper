#include <stdio.h>
#include "ctrlmus.h"
#include "point.h"
#include "msapi.h"

extern vector<vector<int> > map;

MOUSEINPUT lDown , lUp , rDown , rUp;
INPUT inputMouseDown_L , inputMouseUp_L ,
      inputMouseDown_R , inputMouseUp_R;

void initMouse(void){

    lDown.mouseData = lUp.mouseData
        = rDown.mouseData = rUp.mouseData = 0;

    lDown.time = lUp.time
        = rDown.time = rUp.time = 0;

    lDown.dwExtraInfo = lUp.dwExtraInfo
        = rDown.dwExtraInfo = rUp.dwExtraInfo = 0;

    lDown.dwFlags = MOUSEEVENTF_LEFTDOWN;
    lUp.dwFlags = MOUSEEVENTF_LEFTUP;
    rDown.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    rUp.dwFlags = MOUSEEVENTF_RIGHTUP;

    inputMouseDown_L.type = inputMouseUp_L.type
        = inputMouseDown_R.type = inputMouseUp_R.type = INPUT_MOUSE;

    inputMouseDown_L.mi = lDown;
    inputMouseUp_L.mi = lUp;
    inputMouseDown_R.mi = rDown;
    inputMouseUp_R.mi = rUp;

    return;
}

void click(POINT pt , int type) {
    POINT mus;

    if(!~pt.x && !~pt.y){
        mus.y = WINLOCY;
        mus.x = WINLOCX;
    }
    else if(PTINVEC(pt , map)){
        mus.y = CellCenY(pt.y);
        mus.x = CellCenX(pt.x);
    }
    else
        puts("error click pos") , exit(1);

    SetCursorPos(mus.x , mus.y);

    lDown.dx = lUp.dx = rDown.dx = rUp.dx = mus.x;
    lDown.dy = lUp.dy = rDown.dy = rUp.dy = mus.y;

    INPUT event[4] = {
        inputMouseDown_L,
        inputMouseDown_R,
        inputMouseUp_L,
        inputMouseUp_R
    };

    int i ,
        cond[] = {
            type == BOTH || type == LEFT ,
            type == BOTH || type == RIGHT ,
            type == BOTH || type == LEFT ,
            type == BOTH || type == RIGHT
        };

    UINT rtn;
    for(i = 0 ; i < sizeof(cond) / sizeof(int) ; i++){
        if(cond[i] &&
                !SendInput(1, &event[i] , sizeof(INPUT)))
            puts("SendInput Error") , exit(0);
    }

    if(type == RIGHT)
        Dprintf("flag @ (%d , %d)\n" , pt.y , pt.x);

    if(type == LEFT)
        Dprintf("open @ (%d , %d)\n" , pt.y , pt.x);

    printTab(map , true);
    return;
}

void clickLeft(POINT p){
    return click(p , LEFT);
}

void clickRight(POINT p){
    return click(p , RIGHT);
}

void clickBoth(POINT p){
    return click(p , BOTH);
}
