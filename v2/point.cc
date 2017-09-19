#include "point.h"
#include "mark.h"

#include <stdio.h>
#include <stdlib.h>

POINT **adjPts(POINT pt , vector<vector<int> > vec){
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

int cnt_map(int what, vector<vector<int> > mat) {
    int i , j, rtn = 0;
    if(!(what <= ERR && what >= UNK))
        puts("cnt_map Error") , exit(0);

    RPT2V(mat , i , j)
        if(mat[i][j] == what) rtn += 1;

    return rtn;
}

int cnt_adj(POINT pt , int type , vector<vector<int> > mat) {
    int rtn = 0;
    RPTP(mat , pt , p ,
            { if(PTON(p , mat) == type) rtn++; });
    return rtn;
}

int cnt_adj_mrk(POINT pt , vector<vector<int> > mat){
    int rtn = 0;
    RPTP(mat , pt , p , {
        if(HASBOMB(mat , p)) rtn++;
    });
    return rtn;
}
