#ifndef POINT_H
#define POINT_H

#include <vector>
using namespace std;

#ifndef _WINDOWS_H
struct POINT{ int x , y; };
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

POINT **adjPts(POINT pt , vector<vector<char> > vec);

#endif
