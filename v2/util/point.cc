#include "point.h"
#include <stdio.h>
#include <stdlib.h>
POINT **adjPts(POINT pt , vector<vector<char> > vec){
    POINT **rtn =
        (POINT**)calloc(8 , sizeof(POINT*)) ,
        d[] = {
            {0 , -1} , { 1 , -1} ,
            {1 ,  0} , { 1 ,  1} ,
            {0 ,  1} , {-1 ,  1} ,
            {-1 , 0} , {-1 , -1} ,
        };
    if(!rtn) puts("calloc failed!") , exit(0);
    int i , idx = 0;
    for(i = 0 ; i < 8 ; i++){
        POINT tmp = {
            pt.x + d[i].x ,
            pt.y + d[i].y
        };

        if(PTINVEC(tmp , vec)){
            rtn[idx] =
                (POINT*)malloc(sizeof(POINT));
            rtn[idx]->x = tmp.x;
            rtn[idx]->y = tmp.y;
            idx++;
        }
    }
    return rtn;
}
