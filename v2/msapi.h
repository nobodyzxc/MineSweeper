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

#define Dprintf(format , ...) ;//fprintf(stderr , format , __VA_ARGS__)
#define Dputs(str) ;//fprintf(stderr , "%s\n" , str)

void initApi(int y , int x);

void showGamePane();

int getColor(COLORREF);

void printTab(vector<vector<int> > tab , bool move);

int adjPtNum(POINT);

void w_usleep(__int64 usec);
#endif
