void initMap();
int Gaming(void);
int direct();
int dirSng(POINT);
void addFlgArd(POINT);
void cpMaptoRsnMap();
int suppose(int);
int rsning(POINT);
int rsnExpn(POINT , int);
int cntMap(int , int matrix[][MAPWD]);
int adjCnt(POINT , int , int mata[][MAPWD]);
int adjCntMrk(POINT pt , int mat[][MAPWD]);
void printWithExh(int tab[][MAPWD]);
void analyAftClk(POINT pt , int);

int need_flag(void);
int check_if_right(POINT argpt);
void print_map(const char *matrix_name);
int resuppose(void);

int dfs_rsning(void);
void dfs(POINT);
void gessClk(void);
void initMouse(void);
struct notation C(int P , int C);
void addC(struct notation *beadd , struct notation add);
void dfs(POINT);
void gessClk(void);
