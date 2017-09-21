#ifndef MARK_H
#define MARK_H
const int
    UNK = 0 , ONE = 1 , TWO = 2 ,
    THR = 3 , FOR = 4 , FIV = 5 ,
    SIX = 6 , SVN = 7 , EAT = 8 ,
    FLG = 9 , SAF = 10, ERR = 11;

const char MSSYM[] = {
    [UNK] = '.' ,
    [ONE] = ONE + '0',
    [TWO] = TWO + '0',
    [THR] = THR + '0',
    [FOR] = FOR + '0',
    [FIV] = FIV + '0',
    [SIX] = SIX + '0',
    [SVN] = SVN + '0',
    [EAT] = EAT + '0',
    [FLG] = 'F' ,
    [SAF] = '0' ,
    [ERR] = 'X'
};

#define HASBOMB(mp , p) ((mp)[(p).y][(p).x] >= ONE \
                        && (mp)[(p).y][(p).x] <= EAT)

#define UNOPEN(mp , p) ((mp)[(p).y][(p).x] == UNK \
                        || ((mp)[(p).y][(p).x]) == FLG)

#define OPENED(mp , p) (!UNOPEN(mp , p))

#endif
