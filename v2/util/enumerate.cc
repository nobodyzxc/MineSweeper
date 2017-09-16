#include<iostream>
#include<vector>
#include<cstdlib>
#include<cstdio>
#include<cmath>
using namespace std;

int counts = 0;

typedef unsigned int uint;

struct POINT{ int x , y; };

vector<vector<char> > m;
vector<vector<int> > freq , init;

#define PTINMAP(p , m) \
    ((p).x < (int)m[0].size() && (p).x >= 0 \
     && (p).y < (int)m.size() && (p).y >= 0)

#define PTON(p , mp) ((mp)[(p).y][(p).x])

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
    for(uint i = 0 ; i < m.size() ; i++)
        for(uint j = 0 ; j < m[i].size() ; j++)
            if(m[i][j] == type) cnt++;
    return cnt;
}

void input(){
    string s;
    while(cin >> s){
        vector<char> v;
        vector<int> f;
        for(uint i = 0 ; i < s.size() ; i++)
            v.push_back(s[i]) , f.push_back(0);
        m.push_back(v) ,
            freq.push_back(f) ,
            init.push_back(f);
    }
}

void output(){
    char fmt[2][30];
    int maxv = 0 , w;
    for(uint i = 0 ; i < freq.size() ; i++)
        for(uint j = 0 ; j < freq[i].size() ; j++)
            maxv = maxv > freq[i][j] ? maxv : freq[i][j];
    w = (int)log10(maxv) + 1;
    sprintf(fmt[0] , "\033[1;33m%%%dd\033[0m " , w);
    sprintf(fmt[1] , "%%%dc " , w);
    for(uint i = 0 ; i < freq.size() ; i++){
        for(uint j = 0 ; j < freq[i].size() ; j++){
            if(freq[i][j])
                printf(fmt[0] , freq[i][j]);
            else
                printf(fmt[1] , m[i][j]);
        }
        cout << endl;
    }
}

int ctoi(char c){
    return c >= '0' and c <= '9' ? c - '0' : -1;
}

template <typename T>
void printV(vector<vector<T> > mat){
    return;
    for(int i = 0 ; i < mat.size() ; i++ , cout << endl)
        for(int j = 0 ; j < mat[i].size() ; j++)
            cout << mat[i][j] << ' ';
    cout << endl;
}


void addrslt(vector<vector<char> > mat){
    counts += 1;
    //puts("result:");
    printV(mat);
    for(int i = 0 ; i < mat.size() ; i++)
        for(int j = 0 ; j < mat[i].size() ; j++)
            if(mat[i][j] == 'X') freq[i][j] += 1;
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
    //printf("Suppose @ %d %d\n" , y , x);
    POINT pt = {x , y};
    RPTP(pt , p , {
        //printf("arpt %d %d(%d)\n" ,
        //        p.y , p.x , ctoi(PTON(p , m)));
        if(isdigit(m[p.y][p.x])){
            int bomb = adjCnt(p , 'X' , m);
            int ukwn = adjCnt(p , '.' , m);
        //    printf("arpt %d %d(%d):b %d , u %d\n" ,
        //            p.y , p.x , ctoi(PTON(p , m)) ,
        //            bomb , ukwn);
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
    //printf("fill %c ard (%d , %d)\n" ,
    //sym , loc.y , loc.x);
    vector<POINT> vp;
    RPTP(loc , p , {
        if(m[p.y][p.x] == '.'){
            m[p.y][p.x] = sym;
            if(not legalSuppose(p.y , p.x))
                return false;
            vp.push_back(p);
        }
    });
    //printV(m);
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
            int bomb = adjCnt(p , 'X' , m);
            int ukwn = adjCnt(p , '.' , m);
            if(ukwn != 0){
                if(bomb == ctoi(m[p.y][p.x]))
                    if(not fill('_' , p , mrk))
                        return false;
                else if(ukwn + bomb == ctoi(m[p.y][p.x]))
                    if(not fill('X' , p , mrk))
                        return false;
            }
        }
    });
    return true;
}

void copy(vector<vector<char> > &bak ,
        vector<vector<char> > &t){
    for(uint i = 0 ; i < t.size() ; i++){
        vector<char> s;
        for(uint j = 0 ; j < t[i].size() ; j++)
            s.push_back(t[i][j]);
        bak.push_back(s);
    }
}

void recover(vector<vector<char> > &o ,
        vector<vector<char> > &t){
    for(uint i = 0 ; i < t.size() ; i++)
        for(uint j = 0 ; j < t[i].size() ; j++)
            o[i][j] = t[i][j];
}

void dfs(int y , int x){
    char it[3] = "_X";
    POINT nxt = next(y , x);
    if(m[y][x] == '.'){
        for(int i = 0 ; i < 2 ; i++){
            m[y][x] = it[i];
            //printf("%d %d sup %c\n" , y , x , m[y][x]);
            //printf("legal? %d\n" , legalSuppose(y , x));
            //printV(m);
            if(legalSuppose(y , x)){
                vector<vector<char> > bak;
                copy(bak , m);
                vector<vector<int> > mrk = init;
                if(expand(y , x , mrk)){
                    if(allCnt('.' , m) == 0)
                        addrslt(m);
                    else{
                        if(isNULL(nxt)) return;
                        else dfs(nxt.y , nxt.x);
                    }
                }
                recover(m , bak);
            }
            //else{
            //    puts("illeage sup");
            //    printf("dfs %d %d sup %c\n" ,
            //            y , x , m[y][x]);
            //    printV(m);
            //}
            //printf("dfs %d %d sup %c\n" , y , x , m[y][x]);
            //puts("recoverd:");
            //printV(m);
        }
        m[y][x] = '.';
    }
    else{
        if(isNULL(nxt)) return;
        else dfs(nxt.y , nxt.x);
    }
}

int main(){
    input();
    dfs(0 , 0);
    output();
    printf("counts = %d\n" , counts);
}
