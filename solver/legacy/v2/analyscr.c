#include"mouse.h"

#define ISSAF(_hdc , p) \
    (dclr = GetPixel(_hdc , CellLocX(p.x) + 1 , \
                     CellLocY(p.y) + 1)) == SAF_RGB

#define  ISFLG(_hdc , p) \
    (dclr = GetPixel(_hdc , CellLocX(p.x) + 4 , \
                     CellLocY(p.y) + 11)) == FLG_RGB

#define printClr(clr) \
{ int r = GetRValue(clr) , \
    g = GetGValue(clr) , \
    b = GetBValue(clr);\
    printf("RGB(%d , %d , %d)\n" , r , g , b); \
}

int analySpt(POINT pt , bool update){

    if(PTON(pt , map) != UNK)
        return PTON(pt , map);

    COLORREF dclr;
    HDC _hdc = GetDC(NULL);
    if(!_hdc) puts("analyMap connot getDc") , exit(0);
    PTON(pt , map) =
        getColor(GetPixel(_hdc ,
                    CellIdtX(pt.x) ,
                    CellIdtY(pt.y)));

    if(PTON(pt , map) == UNK){
        if(ISSAF(_hdc , pt)){
            PTON(pt , exh) = true;
            PTON(pt , map) = SAF;
        }
        else if(ISFLG(_hdc , pt)){
            PTON(pt , exh) = true;
            PTON(pt , map) = FLG;
        }
    }
    ReleaseDC(NULL , _hdc);
    return PTON(pt , map);
}

int analyRecr(POINT pt , int visit[][MAPWD]) {

    int ptIdx = 0 , rtn = 0;
    POINT *adjptr;
    while (adjptr = adjPts(pt , ptIdx++)){

        if (visit[adjptr->y][adjptr->x])
            continue;

        visit[adjptr->y][adjptr->x] = 1;

        if (map[adjptr->y][adjptr->x] == UNK)
            analySpt(*adjptr , true);

        if(PTON(*adjptr , map) == SAF)
            analyRecr(*adjptr , visit);

        free(adjptr);
    }
//    printf("analyRecr @ (%d,%d) done.\n" , pt.y , pt.x);
    return rtn;
}

void analyAftClk(POINT pt , int type){

    usleep(500000);
    //wait result after click

    if(!PTINMAP(pt))
        puts("error analy pos") , exit(0);

    POINT *adjptr;
    int ptIdx = 0;
    int visit[MAPHI][MAPWD];
    memset(visit, 0, sizeof(visit));
    if(type == BOTH){
        PTON(pt , visit) = true;
        analyRecr(pt , visit);
        direct();
    }
    else if(type == LEFT){
        analySpt(pt , true);
        if(HASBOMB(map , pt)){
            dirSng(pt);
        }
        else if(map[pt.y][pt.x] == SAF){
            PTON(pt , visit) = true;
            analyRecr(pt , visit);
            puts("ANALY OF LEFT CLK @ SAF DONE");
            direct();
        }
    }
    else if(type == RIGHT){
        while (adjptr = adjPts(pt , ptIdx++)){
            dirSng(*adjptr);
            free(adjptr);
        }
    }
}

#define dcDebug() \
    if(map[y][x] == -1){ \
        printf("(%d , %d)->" , y , x); \
        printf("%d %d -> " , CellIdtY(y) , CellIdtX(x)); \
        COLORREF c = \
        GetPixel(_hdc , CellIdtX(x) , CellIdtY(y)); \
        int r = GetRValue(c) , \
        g = GetGValue(c) , \
        b = GetBValue(c);\
        printf("(%d %d %d)\n" , r , g , b);\
    }

void analyMap(bool update){
    int x , y;
    COLORREF dclr;
    HDC _hdc = GetDC(NULL);
    if(!_hdc) puts("analyMap connot getDc") , exit(0);

    for(y = 0 ; y < 16 ; y++)
        for(x = 0 ; x < 30 ; x++){
            POINT pt = {x , y};

            if(PTON(pt , map) != UNK)
                continue;

            PTON(pt , map) =
                getColor(GetPixel( _hdc ,
                            CellIdtX(x) ,
                            CellIdtY(y)));

            if(PTON(pt , map) == UNK){
                if(ISSAF(_hdc , pt)){
                    PTON(pt , exh) = true;
                    PTON(pt , map) = SAF;
                }
                else if(ISFLG(_hdc , pt)){
                    PTON(pt , exh) = true;
                    PTON(pt , map) = FLG;
                }
            }
#if DEBUG
            dcDebug();
#endif
        }
    ReleaseDC(NULL , _hdc);
    return;
}
