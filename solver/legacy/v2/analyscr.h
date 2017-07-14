#ifndef ANALYSCR_H
#define ANALYSCR_H

#define RGB_TO_RGB(p) \
    ((((p) & 0xFF) << 16) \
     | ((p) & 0xFF00) \
     | (((p) & 0xFF0000) >> 16) )

#define GetPx(x , y , update) \
    (RGB_TO_RGB(GetBMptr(update)[(y) * lWidth + (x)]))

#define ISSAF(p , update) \
    (dclr = GetPx(CellLocX((p).x) + 1 , \
                  CellLocY((p).y) + 1 , update)) == SAF_RGB

#define  ISFLG(p , update) \
    (dclr = GetPx(CellLocX((p).x) + 4 , \
                  CellLocY((p).y) + 11 , update)) == FLG_RGB

#define printClr(clr) \
{ int r = GetRValue(clr) , \
    g = GetGValue(clr) , \
    b = GetBValue(clr);\
    printf("RGB(%d , %d , %d)\n" , r , g , b); \
}

int analySpt(POINT pt , bool update);

int analyRecr(POINT pt , int visit[][MAPWD]);

void analyAftClk(POINT pt , int type);

void analyMap(bool update);

LPDWORD GetBMptr(bool update);

#endif
