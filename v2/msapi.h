#ifndef MINEAPI_H
#define MINEAPI_H


#include<windows.h>
#include<vector>
using namespace std;

#include"game.h"
#include"mark.h"
#include"analyscr.h"
#include"ctrlmus.h"
#include"point.h"

#define Dprintf(format , ...) (0)
#define Dputs(str) (0)

void initApi(int y , int x);

void showGamePane();

int getColor(COLORREF);

void printTab(vector<vector<int> > tab , bool move);

int adjPtNum(POINT);

void w_usleep(__int64 usec);
#endif
