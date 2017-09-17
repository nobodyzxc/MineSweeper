#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <cstring>
#include "util.cc"

typedef unsigned long long ull;
typedef unsigned int uint;

using namespace std;

int counts = 0 , ref_flg = 0 ,
    donedots = 0 , dfscount = 0 ,
    remain_flags = 0 , omitcount = 0;


struct POINT{ int x , y; };

vector<vector<char> > m;
vector<vector<ull> > freq;

#define PTINMAP(p , m) \
    ((p).x < (int)m[0].size() && (p).x >= 0 \
     && (p).y < (int)m.size() && (p).y >= 0)

#define PTON(p , mp) ((mp)[(p).y][(p).x])

#define RPT2V(v , i , j) \
    for(uint i = 0 ; i < v.size() ; i++) \
        for(uint j = 0 ; j < v[i].size() ; j++)

#define NRPT2V(v , i , j , stat0 , stat1) \
    for(uint i = 0 ; i < v.size() ; i++){ \
        for(uint j = 0 ; j < v[i].size() ; j++){ \
            stat0; \
        } \
        stat1; \
    }

POINT **adjPts(POINT pt){
    POINT **rtn =
        (POINT**)calloc(8 , sizeof(POINT*)) ,
        d[] = {
            {0 , -1} , { 1 , -1} ,
            {1 ,  0} , { 1 ,  1} ,
            {0 ,  1} , {-1 ,  1} ,
            {-1 , 0} , {-1 , -1} ,
        };
    if(!rtn) puts("calloc failed!") , exit(0);
    int i , idx = 0;
    for(i = 0 ; i < 8 ; i++){
        POINT tmp = {
            pt.x + d[i].x ,
            pt.y + d[i].y
        };

        if(PTINMAP(tmp , m)){
            rtn[idx] =
                (POINT*)malloc(sizeof(POINT));
            rtn[idx]->x = tmp.x;
            rtn[idx]->y = tmp.y;
            idx++;
        }
    }
    return rtn;
}

#define RPTP(begp , p_var , stat) \
    do{ \
        int _pidx; \
        POINT **_pp = adjPts(begp); \
        for(_pidx = 0 ; _pidx < 8 && \
                _pp[_pidx] ; _pidx++){ \
            POINT p_var = *_pp[_pidx]; \
            stat; \
        } \
        for(_pidx = 0 ; _pidx < 8 ; _pidx++) \
        free(_pp[_pidx]); \
        free(_pp); \
    }while(0);

int adjCnt(POINT pt , char type ,
        vector<vector<char> > mat){
    int rtn = 0;
    RPTP(pt , p , { if(PTON(p , mat) == type) rtn++; });
    return rtn;
}

int allCnt(char type , vector<vector<char> > mat){
    int cnt = 0;
    RPT2V(m , i , j)
        if(m[i][j] == type) cnt++;
    return cnt;
}

bool omit(int y , int x){
    if(m[y][x] != '.') return false;
    POINT p = {x , y};
    int rtn = 0;
    for(int i = 0 ; i <= 8 ; i++)
        rtn += adjCnt(p , i + '0' , m);
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
        vector<char> v;
        vector<ull> f;
        for(uint i = 0 ; i < s.size() ; i++)
            v.push_back(s[i]) , f.push_back(0);
        m.push_back(v) , freq.push_back(f);
    }

    RPT2V(m , i , j)
        if(omit(i , j)) m[i][j] = 'O';
    omitcount = allCnt('O' , m);
}

void output(){
    char fmt[4][30] , sm[100];
    int maxv = 0 , w , hun = 0;
    RPT2V(freq , i , j){
        maxv = maxv > freq[i][j] ? maxv : freq[i][j];
        if(freq[i][j] > 99 and freq[i][j] < 1000) hun = 1;
    }
    w = (int)log10(maxv) + 1;
    if(w < 2) w = 2;
    if(w > 3 and not hun) w = 2;
    else w = 3;
    sprintf(fmt[0] , "\033[1;30m%%%dc\033[0m " , w);
    sprintf(fmt[1] , "%%%dc " , w);
    sprintf(fmt[2] , "\033[1;33m%%%dlld\033[0m " , w);
    NRPT2V(freq , i , j ,
            if(not ref_flg and m[i][j] == 'O')
                printf(fmt[0] , 'O');
            else if(m[i][j] != '.')
                printf(fmt[1] , m[i][j]);
            else if(freq[i][j] < 999)
                printf(fmt[2] , freq[i][j]);
            else{
                int e = (int)log10(freq[i][j]);
                int cap = freq[i][j] / (ull)pow(10.0f , (double)e);
// uncommet for more detail
//#define TRY_BUG
#ifdef TRY_BUG
                //printf("\ne -> %d -->" , e);
#endif
                sprintf(sm , "\x1b[32m%d\033[01;31m%d\033[0m" ,
#ifdef TRY_BUG
                        freq[i][j] / (ull)pow(10.0f , (double)e) , e);
#else
                        cap , e); // ^^ bug
#endif

#ifdef TRY_BUG
                //puts(sm);
#endif
                if(w - (int)log10(e) - 2){
                    sprintf(fmt[3] , "%%%dc%%s " , w - (int)log10(e) - 2);
                    printf(fmt[3] , ' ' , sm);
                }
                else{
                    sprintf(fmt[3] , "%%s ");
                    printf(fmt[3] , sm);
                }
            } ,
            cout << endl);
}

int ctoi(char c){
    return c >= '0' and c <= '9' ? c - '0' : -1;
}

template <typename T>
void printV(vector<vector<T> > mat){
    //return;
    NRPT2V(mat , i , j ,
            cout << mat[i][j] << ' ' ,
            cout << endl);
}


void addrslt(vector<vector<char> > mat){
    if(ref_flg and remain_flags < 0) // from !=
        return;
    counts += 1;
    //printV(mat);
    //printf("%d flag left\n" , remain_flags);
    RPT2V(mat , i , j)
        if(mat[i][j] == 'F' || mat[i][j] == 'O'){
            if(ref_flg){
                if(omitcount < remain_flags)
                    printf("error of c(%lld , %d)\n" ,
                            omitcount , remain_flags) ,
                        exit(0);
                freq[i][j] += choose(omitcount , remain_flags);
            }
            else if(mat[i][j] == 'F'){
                if(omitcount == 0) freq[i][j] += 1;
                else if(remain_flags <= 0) freq[i][j] += 1;
                else if(remain_flags <= omitcount)
                    freq[i][j] +=
                        choose(omitcount , remain_flags);
                else
                    printf("error of c(%lld , %d)\n" ,
                            omitcount , remain_flags) ,
                        exit(0);
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

bool legalSuppose(int y , int x){
    POINT pt = {x , y};
    RPTP(pt , p , {
        if(isdigit(m[p.y][p.x])){
            int bomb = adjCnt(p , 'F' , m);
            int ukwn = adjCnt(p , '.' , m);
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
    RPTP(loc , p , {
        if(m[p.y][p.x] == '.'){
            m[p.y][p.x] = sym;
            if(not legalSuppose(p.y , p.x))
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
    RPTP(loc , p , {
        if(isdigit(m[p.y][p.x])){
            int bomb = adjCnt(p , 'F' , m);
            int ukwn = adjCnt(p , '.' , m);
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
            if(legalSuppose(y , x)){
                vector<vector<char> > bak = m;
                vector<vector<int> > mrk(m.size() ,
                        vector<int>(m[0].size() , 0));
                int remain_flags_bak;
                if(ref_flg)
                    remain_flags_bak = remain_flags;
                if(expand(y , x , mrk))
                    if(allCnt('.' , m) == 0) addrslt(m);
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

int main(int argc , char *argv[]){
    if(argc > 1)
        if(!freopen(argv[1] , "r" , stdin))
            printf("read %s failed\n" , argv[1]) ,
                exit(0);
        else printf("read %s\n" , argv[1]);

    if(argc > 2) ref_flg = true;

    input();


    if(ref_flg)
        remain_flags =
            atoi(argv[2]) - allCnt('F' , m);

    dfs(0 , 0 , 0.0f , 100.0f);
    output();

    printf("counts = %d , dfs = %d\n" , counts , dfscount);
}
