#ifndef SOLVE_H
#define SOLVE_H

#include<vector>
using namespace std;

void init_map();

int play();

int direct();

int dirSng(POINT pt);

void fancyClickLeft(POINT p);

void fancyClickBoth(POINT p);

void addFlgArd(POINT pt);

void analyAftClk(POINT pt , int type);

int rsning(POINT pt);

int rsnExpn(POINT pt , int type);

int suppose(int what);

void printTab(vector<vector<int> > tab);

int cnt_adj_mrk(POINT pt , vector<vector<int> > mat);

#endif
