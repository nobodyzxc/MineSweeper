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

void initApi();

void showGamePane();

int getColor(COLORREF);

void printTab(int tab[][MAPWD]);

POINT **adjPts(POINT);

int adjPtNum(POINT);

void w_usleep(__int64 usec);
#endif
