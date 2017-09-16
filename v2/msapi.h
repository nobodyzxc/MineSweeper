#ifndef MINEAPI_H
#define MINEAPI_H

#include<windows.h>

#include"game.h"
#include"mark.h"
#include"analyscr.h"
#include"ctrlmus.h"

#define PTINMAP(p) ((p).x < MAPWD && (p).x >= 0 \
                        && (p).y < MAPHI && (p).y >= 0)

#define HASBOMB(mp , p) ((mp)[(p).y][(p).x] >= ONE \
                        && (mp)[(p).y][(p).x] <= EAT)

#define PTON(p , mp) ((mp)[(p).y][(p).x])

#define UNOPEN(mp , p) ((mp)[(p).y][(p).x] == UNK \
                        || ((mp)[(p).y][(p).x]) == FLG)

#define OPENED(mp , p) (!UNOPEN(mp , p))

#define RPTP(point , p_var , pidx_var , stat) \
    do{ \
        int pidx_var; \
        POINT **p_var = adjPts(point); \
        for(pidx_var = 0 ; pidx_var < 8 && \
                p_var[pidx_var] ; pidx_var++){ \
            stat; \
        } \
        for(pidx_var = 0 ; pidx_var < 8 ; pidx_var++) \
            free(p_var[pidx_var]); \
        free(p_var); \
    }while(0);

void initApi();

void showGamePane();

int getColor(COLORREF);

void printTab(int tab[][MAPWD]);

POINT **adjPts(POINT);

int adjPtNum(POINT);

void w_usleep(__int64 usec);
#endif
