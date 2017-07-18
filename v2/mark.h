#ifndef MARK_H
#define MARK_H
enum {
    UNK , ONE , TWO , THR , FOR , FIV ,
    SIX , SVN , EAT , FLG , SAF , ERR
};

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
#endif
