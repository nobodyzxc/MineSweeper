#ifndef MINEAPI_H
#define MINEAPI_H


#include<windows.h>
#include<vector>
using namespace std;

#include"game.h"
#include"mark.h"
#include"point.h"
#include"analyscr.h"
#include"ctrlmus.h"

void initApi(int y , int x);

void showGamePane();

int getColor(COLORREF);

//TODO: add template for adjPTs
POINT **adjPts(POINT , vector<vector<int> >);

int adjPtNum(POINT);

void w_usleep(__int64 usec);
#endif
