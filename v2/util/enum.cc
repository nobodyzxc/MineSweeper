#include<iostream>
#include<vector>
#include<cstdlib>
#include<cstdio>
using namespace std;

typedef unsigned int uint;

struct POINT{ int x , y; };

vector<vector<char> > m;
vector<vector<int> > freq;
int dots = 0;

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
            {-1 , 0} , {-1 , -1}
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

#define RPTP(point , p_var , pidx_var , stat) \
    do{ \
        int pidx_var; \
        POINT **p_var = adjPts(point); \
        for(pidx_var = 0 ; pidx_var < 8 && \
                p[pidx_var] ; pidx_var++){ \
            stat; \
        } \
        for(pidx_var = 0 ; pidx_var < 8 ; pidx_var++) \
            free(p[pidx_var]); \
        free(p); \
    }while(0);

int adjCnt(POINT pt , char type ,
        vector<vector<char> > mat){
    int rtn = 0;
    RPTP(pt , p , idx , {
        if(PTON(*p[idx] , mat) == type) rtn++;
    });
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
        m.push_back(v) , freq.push_back(f);
    }
}

void output(){
    for(uint i = 0 ; i < m.size() ; i++){
        for(uint j = 0 ; j < m[i].size() ; j++){
            POINT p;
            p.y = i , p.x = j;
            cout << adjCnt(p , '.' , m);
        }
        cout << endl;
    }
}

int get(char c){
    if(c >= '0' and c <= '9')
        return c - '0';
    if(c == '.')
        return -1;
    return -2;
}

void dfs(int y , int x){
    return;
    for(uint i = 0 ; i < m.size() ; i++){
        for(uint j = 0 ; j < m[i].size() ; j++){
            POINT p;
            p.y = i , p.x = j;
            cout << adjCnt(p , '.' , m);
        }
        cout << endl;
    }
}

int main(){
    input();
    dots = allCnt('.' , m);
    dfs(0 , 0);
    output();
}
