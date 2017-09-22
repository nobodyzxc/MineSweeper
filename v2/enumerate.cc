#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <unistd.h>

#include "mark.h"
#include "point.h"
#include "util.cc"

typedef unsigned long long ull;
typedef unsigned int uint;

using namespace std;

int counts = 0 , ref_flg = 0 ,
    dfscount = 0 , remain_flags = 0 , omitcount = 0;

vector<vector<char> > m;
vector<vector<sciNotat> > freq;

void init_env(int flgs){
    counts = 0;
    ref_flg = 1;
    remain_flags = flgs;
    dfscount = 0 , omitcount = 0;
    for(uint i = 0 ; i < m.size() ; i++)
        m[i].clear();
    m.clear();
    for(uint i = 0 ; i < freq.size() ; i++)
        freq[i].clear();
    freq.clear();
}

bool omit(int y , int x){
    if(m[y][x] != '.') return false;
    POINT p = {x , y};
    int rtn = 0;
    for(int i = 0 ; i <= 8 ; i++)
        rtn += cnt_adj(p , char(i + '0') , m);
    return rtn == 0;
}

void input(){
    string s;
    while(getline(cin , s)){
        size_t pos;
        while((pos = s.find(' ')) != string::npos)
            s.erase(pos , 1);
        while((pos = s.find('\r')) != string::npos)
            s.erase(pos , 1); // f*** windows
        if(s.size() == 0) continue;
        m.push_back(vector<char>(s.begin() , s.end()));
        freq.push_back(vector<sciNotat>(s.size() , sciNotat()));
    }
    RPT2V(m , i , j)
        if(omit(i , j)) m[i][j] = 'O';
    omitcount = cnt_map('O' , m);
}

void output(){
    char fmt[3][60] , ofmt[3][60] , sm[200];
    sciNotat maxv; int w;

    printf("\033[40A\033[17B");
    RPT2V(freq , i , j){
        maxv = maxv.greaterThan(freq[i][j]) ? maxv : freq[i][j];
    }
    w = maxv.exponent + 1;
    w = int(log10(w)) + 2;
    if(w == 2 || w >= 4) w = 3;
    sprintf(ofmt[0] , "\033[1;30m%%%dc\033[0m " , w);
    sprintf(ofmt[1] , "\033[1;30m%%%dlld\033[0m " , w);

    sprintf(fmt[0] , "%%%dc " , w);
    sprintf(fmt[1] , "\033[1;33m%%%dlld\033[0m " , w);

    NRPT2V(freq , i , j , {
        if(not ref_flg and m[i][j] == 'O'){
            printf(ofmt[0] , '.');
        }
        else if(m[i][j] != '.' and m[i][j] != 'O'){
            printf(fmt[0] , m[i][j]);
        }
        else if(freq[i][j].exponent < 3){
            printf((m[i][j] == 'O' ? ofmt[1] : fmt[1]) ,
                    int(freq[i][j].mantissa * pow(10 , freq[i][j].exponent)));
        }
        else{
            sprintf(sm , (m[i][j] == 'O' ?
                     "\x1b[1;30m%d\033[0;37m%d\033[0m" :
                     "\x1b[32m%d\033[01;31m%d\033[0m") ,
                    int(freq[i][j].mantissa) , freq[i][j].exponent);
            int wsp = w - int(log10(freq[i][j].exponent)) - 2;
            if(wsp){
                sprintf(fmt[2] , "%%%dc%%s " , wsp);
                printf(fmt[2] , ' ' , sm);
            }
            else{
                sprintf(fmt[2] , "%%s ");
                printf(fmt[2] , sm);
            }
        }
        } ,
        cout << endl);
}

int ctoi(char c){
    return c >= '0' and c <= '9' ? c - '0' : -1;
}

bool legal_allmap(vector<vector<char> > mat){
    for(uint i = 0 ; i < mat.size() ; i++)
        for(uint j = 0 ; j < mat[i].size() ; j++){
            POINT p = {j , i};
            if(isdigit(mat[p.y][p.x])){
                int bomb = cnt_adj(p , 'F' , mat);
                int ukwn = cnt_adj(p , '.' , mat);
                if(ctoi(PTON(p , mat)) < bomb) return false;
                if(ctoi(PTON(p , mat)) > bomb
                        and ukwn == 0) return false;
                if(ctoi(PTON(p , mat)) > bomb + ukwn)
                    return false;
            }
        }
    return true;
}

void addrslt(vector<vector<char> > mat){
    if(ref_flg and remain_flags < 0) // from !=
        return;

    if(not legal_allmap(mat))
        puts("fuck , fake!") , exit(0);

    counts += 1;

    sciNotat nk;

    if(omitcount < remain_flags) return;

    if(ref_flg)
        nk = choose(omitcount , remain_flags);

    RPT2V(mat , i , j)
        if(mat[i][j] == 'F' || mat[i][j] == 'O'){
            if(ref_flg and mat[i][j] == 'O'){
                if(omitcount < remain_flags){
                    printf("error of c(%lld , %d)(0)\n" ,
                            omitcount , remain_flags);
                    return;
                }
                if(remain_flags)
                    freq[i][j].add(nk);
            }
            else if(mat[i][j] == 'F'){
                if(omitcount == 0)
                    freq[i][j].mantissa += 1;
                else if(remain_flags <= 0)
                    freq[i][j].mantissa += 1;
                else if(remain_flags <= omitcount)
                    freq[i][j].add(nk);
                else{
                    printf("error of c(%lld , %d)(1)\n" ,
                            omitcount , remain_flags);
                    return;
                }
                freq[i][j].normalize();
            }
        }
}

#define isNULL(p) ((p).y == -1 or (p).x == -1)
POINT next(int y , int x){
    POINT rtn = {x , y} , null = {-1 , -1};
    rtn.y = y + (x + 1) / m[0].size();
    rtn.x = (x + 1) % m[0].size();
    return rtn.y < m.size() ? rtn : null;
}

#define isdigit(x) (x <= '9' and x >= '0')

bool legal_suppose(int y , int x){
    POINT pt = {x , y};
    RPTP(m , pt , p , {
        if(isdigit(m[p.y][p.x])){
            int bomb = cnt_adj(p , 'F' , m);
            int ukwn = cnt_adj(p , '.' , m);
            if(ctoi(PTON(p , m)) < bomb) return false;
            if(ctoi(PTON(p , m)) > bomb
                    and ukwn == 0) return false;
            if(ctoi(PTON(p , m)) > bomb + ukwn)
                return false;
        }
    });
    return true;
}

bool expand(int y ,int x , vector<vector<int> > mrk);

bool fill(char sym , POINT loc ,
        vector<vector<int> > mrk){
    vector<POINT> vp;
    RPTP(m , loc , p , {
        if(m[p.y][p.x] == '.'){
            m[p.y][p.x] = sym;
            if(not legal_suppose(p.y , p.x))
                return false;
            vp.push_back(p);
            if(sym == 'F' and ref_flg){
                remain_flags--;
                if(remain_flags < 0)
                    return false;
            }
        }
    });
    for(uint i = 0 ; i < vp.size() ; i++){
        if(not expand(vp[i].y , vp[i].x , mrk))
            return false;
    }
    return true;
}

bool expand(int y ,int x , vector<vector<int> > mrk){
    if(mrk[y][x]) return true;
    mrk[y][x] = 1;
    POINT loc = {x , y}; // the point is not number
    RPTP(m , loc , p , {
        if(isdigit(m[p.y][p.x])){
            int bomb = cnt_adj(p , 'F' , m);
            int ukwn = cnt_adj(p , '.' , m);
            if(ukwn != 0){
                if(bomb == ctoi(m[p.y][p.x]))
                    if(not fill('_' , p , mrk))
                        return false;
                else if(ukwn + bomb == ctoi(m[p.y][p.x]))
                    if(not fill('F' , p , mrk))
                        return false;
            }
        }
    });
    return true;
}

void dfs(int y , int x , float done , float part){
    dfscount++;
    char it[3] = "_F";
    POINT nxt = next(y , x);
    if(m[y][x] == '.'){
        for(int i = 0 ; i < 2 ; i++){
            if(it[i] == 'F') remain_flags--;
            m[y][x] = it[i];
            if(legal_suppose(y , x)){
                vector<vector<char> > bak = m;
                vector<vector<int> > mrk(m.size() ,
                        vector<int>(m[0].size() , 0));
                int remain_flags_bak;
                if(ref_flg)
                    remain_flags_bak = remain_flags;
                if(expand(y , x , mrk))
                    if(cnt_map('.' , m) == 0) addrslt(m);
                    else if(isNULL(nxt)) return;
                    else dfs(nxt.y , nxt.x , done , part / 2);
                m = bak;
                if(ref_flg)
                    remain_flags = remain_flags_bak;
            }
            if(it[i] == 'F') remain_flags++;
            done += part / 2;
            progress(done);
        }
        m[y][x] = '.';
    }
    else{
        if(isNULL(nxt)) return;
        else dfs(nxt.y , nxt.x , done , part);
    }
}

#ifdef ENUM_MAIN
int main(int argc , char *argv[]){
    if(argc > 1){
        if(!freopen(argv[1] , "r" , stdin))
            printf("read %s failed\n" , argv[1]) ,
                exit(0);
        else printf("read %s\n" , argv[1]);
    }

    if(argc > 2) ref_flg = true;

    input();

    if(ref_flg)
        remain_flags =
            atoi(argv[2]) - cnt_map('F' , m);

    dfs(0 , 0 , 0.0f , 100.0f);
    output();
    printf("counts = %d , dfs = %d\n" , counts , dfscount);
}
#endif

void
enumerate(vector<vector<int> > map , int flgs){

    init_env(flgs);

    for(unsigned int i = 0 ; i < map.size() ; i++){
        m.push_back(vector<char>(0));
        freq.push_back(vector<sciNotat>(map[i].size() , sciNotat()));
        for(unsigned int j = 0 ; j < map[i].size() ; j++)
            m[i].push_back(MSSYM[map[i][j]]);
    }

    RPT2V(m , i , j)
        if(omit(i , j)) m[i][j] = 'O';
    omitcount = cnt_map('O' , m);
    progress(-1); // init progess bar
    dfs(0 , 0 , 0.0f , 100.0f);
}

void improved_print(
        vector<vector<int> > map ,
        int flgs){
    enumerate(map , flgs);
    output();
    printf("counts = %d , dfs = %d\n" , counts , dfscount);
}

bool
improved_sol(
        vector<vector<int> > map ,
        int flgs ,
        void (*click_left)(POINT p) ,
        void (*click_right)(POINT p)){

    enumerate(map , flgs);

    bool ex_zero = false;
    /* open all zero */
    RPT2V(m , i , j)
        if(m[i][j] == '.' and freq[i][j].isZero()){
            POINT _ = {j , i};
            ex_zero = true , click_left(_);
        }
    output();
    if(ex_zero) return true;

    /* find min and open of max to flag */
    return false;
}
