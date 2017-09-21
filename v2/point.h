#ifndef POINT_H
#define POINT_H

#include "mark.h"
#include <windows.h>
#include <vector>
#include <stdio.h>

using namespace std;

#ifndef _WINDOWS_H
//struct POINT{ int x , y; };
#endif

#define PTINVEC(p , m) \
    ((p).x < (int)m[0].size() && (p).x >= 0 \
     && (p).y < (int)m.size() && (p).y >= 0)

#define PTON(p , mp) ((mp)[(p).y][(p).x])

#define RPTP(vec , begp , p_var , stat) \
    do{ \
        int _pidx; \
        POINT **_pp = adjPts(begp , vec); \
        for(_pidx = 0 ; _pidx < 8 && \
                _pp[_pidx] ; _pidx++){ \
            POINT p_var = *_pp[_pidx]; \
            stat; \
        } \
        for(_pidx = 0 ; _pidx < 8 ; _pidx++) \
        free(_pp[_pidx]); \
        free(_pp); \
    }while(0);

#define RPTHI(i) \
    for(int i = 0 ; i< MAPHI ; i++)

#define RPTMAP(i , j) \
    for(int i = 0 ; i< MAPHI ; i++) \
        for(int j = 0 ; j < MAPWD ; j++)

#define RPT2V(v , i , j) \
    for(unsigned int i = 0 ; i < v.size() ; i++) \
        for(unsigned int j = 0 ; j < v[i].size() ; j++)

#define NRPT2V(v , i , j , stat0 , stat1) \
    for(unsigned int i = 0 ; i < v.size() ; i++){ \
        for(unsigned int j = 0 ; j < v[i].size() ; j++){ \
            stat0; \
        } \
        stat1; \
    }

#define NRPTMAP(i , j , pre_stat , in_stat , post_stat) \
    for(int i = 0 ; i< MAPHI ; i++){\
        pre_stat; \
        for(int j = 0 ; j < MAPWD ; j++){ \
            in_stat; \
        } \
        post_stat; \
    }

template <typename T , typename A>
POINT **adjPts(POINT pt , vector<vector<T , A> > vec){
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

template <typename T>
int cnt_map(T what, vector<vector<T> > mat) {
    int rtn = 0;

    RPT2V(mat , i , j)
        if(mat[i][j] == what) rtn += 1;

    return rtn;
}

template <typename T>
int cnt_adj(POINT pt , T type , vector<vector<T> > mat) {
    int rtn = 0;
    RPTP(mat , pt , p ,
            { if(PTON(p , mat) == type) rtn++; });
    return rtn;
}

#endif
