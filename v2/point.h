#ifndef POINT_H
#define POINT_H

#include <windows.h>
#include <vector>
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

POINT **adjPts(POINT pt , vector<vector<int> > vec);

int cnt_map(int what, vector<vector<int> > mat);

int cnt_adj(POINT pt , int type , vector<vector<int> > mat);

int cnt_adj_mrk(POINT pt , vector<vector<int> > mat);

#endif
