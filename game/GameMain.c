#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<time.h>
#include<sys/timeb.h>
#include<stdbool.h>
#include"GameMain.h"

void printMap(int);
void DebugNum(int);
void run(char , int , int);
int inRange(int , int);
int ArroundNum(int , int);
bool CheckIfWin(void);
void GameOver(int , int);
/* recursively open */
void expand(int , int);

int **G_MAP , G_BOMBCOUNT , G_MAPLN , G_MAPWD;
int **gameStart(int mapLN , int mapWD , int bomb_num){
//int main(void){
//    int mapLN = 0 , mapWD , bomb_num;

    if(mapLN == 0 || mapWD == 0){
        G_MAPLN = 10;
        G_MAPWD = 10;
        G_BOMBCOUNT = 10;
    }
    else{
        G_MAPLN = mapLN;
        G_MAPWD = mapWD;
        G_BOMBCOUNT = bomb_num;
    }

    int i , j , y , x ;
	char mod;

	struct timeb timeBuf;
	ftime(&timeBuf);
	srand(timeBuf.millitm);
	
	if(!(G_MAP = (int**) malloc
                (G_MAPLN * sizeof(void*) + G_MAPLN *G_MAPWD * sizeof(int))))
		exit(1);

	int *tmp = (int *)(G_MAP + G_MAPLN);
	for(i = 0; i != G_MAPLN ; i++ , tmp += G_MAPWD)
		G_MAP[i] = tmp;

	for(i = 0; i < G_MAPLN ; i++)
		for(j = 0; j < G_MAPWD ; j++)
			G_MAP[i][j] = 0;

	for(i = 0; i < G_BOMBCOUNT ; i++){
		while(G_MAP[ (y = rand() % G_MAPLN) ][ (x = rand() % G_MAPWD) ] );
		//map[y][x] = 1;
        setBOMB(G_MAP[y][x]);
	}

	
//	setbuf(stdin, NULL);
//
//	char Input[100];
//
//	while(1){
//		printMap(G_MAPLN, G_MAPWD , 1);
//		while(1){
//			puts("(s/?/f) (y) (x)");
//			fgets(Input , sizeof(Input) , stdin);
//			if(sscanf( Input , "%c %d %d" , &mod , &y , &x) != 3){
//				setbuf(stdin , NULL);
//				printf("\033[1A");
//				printf("\033[K");
//				printf("\033[1A");
//				printf("\033[K");
//				continue;
//			}
//			setbuf(stdin,NULL);
//			if((mod == 'f' || mod == 's' || mod == '?') && y >= 0 && y < G_MAPLN && x >= 0 && x < G_MAPWD)
//				break;
//			else{
//				printf("\033[1A");
//				printf("\033[K");
//				printf("\033[1A");
//				printf("\033[K");
//
//			}
//		}
//		run(mod , y , x);
//
//		if(CheckIfWin(bomb_num , G_MAPLN , G_MAPWD)){
//			free(G_MAP);
//			puts("Find All Land Mines !");
//			exit(0);
//		}
//
//		for(i = 0; i < G_MAPLN + 6 ; i++){
//			printf("\033[1A");
//			printf("\033[K");
//		}
//
//	}	
//
	return G_MAP;
}

void printMap(int gameState){
	int i , j;
	printf("    ");
	for(i = 0; i < G_MAPLN ; i++)
		printf("%d " , i);
	puts("");

	printf("    ");
	for(i = 0; i < G_MAPWD ; i++)
		printf("- ");
	puts("");
	
	for(i = 0; i < G_MAPLN ; i++){
		printf("%d | " , i);
		for(j = 0; j < G_MAPWD ; j++){
            if(hasFLAG(G_MAP[i][j]))
				printf("%c " , 
                        gameState == 0 && hasBOMB(G_MAP[i][j]) ? 'X' : 'F');
            else if(hasQUES(G_MAP[i][j]))
				printf("%c " , 
                        gameState == 0 && hasBOMB(G_MAP[i][j]) ? '*' : '?');
            else if(hasNoMark(G_MAP[i][j]))
                printf("%c " , 
                        gameState == 0 && hasBOMB(G_MAP[i][j]) ? '*' : 'O');
			else if(hasOPENED(G_MAP[i][j])){
				if(ArroundNum(i , j) == 0)
					printf("  ");
				else
					printf("%d ",ArroundNum(i , j));
			}
			else if(G_MAP[i][j] == 9)
				printf("# ");
		}
		printf("| %d", i);
		puts("");
	}
	
	printf("    ");
	for(i = 0; i < G_MAPWD ; i++)
		printf("- ");
	puts("");

	printf("    ");
	for(i = 0; i < G_MAPLN ; i++)
		printf("%d " , i);
	puts("");

	return;
}

void run(char mod , int y , int x){
    if(hasOPENED(G_MAP[y][x])) return;    
	if(mod == 's'){
		if(isNon(G_MAP[y][x])){
            setOPEN(G_MAP[y][x]); 
			//G_MAP[y][x] = 6;
			expand(y , x);
		}
		else if(hasNoMark(G_MAP[y][x]) && hasBOMB(G_MAP[y][x])){
			GameOver(y , x);
		}
	}
	else if(mod == '?'){
		//if(G_MAP[y][x] == 0 || G_MAP[y][x] == 1) G_MAP[y][x] += 2;
        if(hasNoMark(G_MAP[y][x])) setQUES(G_MAP[y][x]);
		else if(hasFLAG(G_MAP[y][x])){
			if(hasBOMB(G_MAP[y][x]))
				G_BOMBCOUNT++;
            setQUES(G_MAP[y][x]);
			//G_MAP[y][x] -= 2;
		}
		else if(hasQUES(G_MAP[y][x])){
            unsetQUES(G_MAP[y][x]);
			//G_MAP[y][x] -= 2;
        }
	}
	else if(mod == 'f'){
		if(hasNoMark(G_MAP[y][x])){
			if(hasBOMB(G_MAP[y][x]))
				G_BOMBCOUNT--;
			setFLAG(G_MAP[y][x]);
		}
		else if(hasQUES(G_MAP[y][x])){
			if(hasBOMB(G_MAP[y][x]))
				G_BOMBCOUNT--;
            setFLAG(G_MAP[y][x]);
		}
		else if(hasFLAG(G_MAP[y][x])){
			if(hasBOMB(G_MAP[y][x]))
				G_BOMBCOUNT++;
			unsetFLAG(G_MAP[y][x]);
		}
	}
	return;
}
void expand(int y , int x){
	if(ArroundNum(y , x))
		return;
    int dy[3] = {1 , 0 , -1} , dx[3] = {1 , 0 , -1};
	int i , j , sy , sx , recur = 0;
    for(i = 0 ; i < 3 ; i++){
        for(j = 0 ; j < 3 ; j++){
            if(i == 1 && j == 1) continue;
            sy = y + dy[j] , sx = x + dx[i];
            if(inRange(sy , sx)){
                if(!G_MAP[sy][sx])
                    setOPEN(G_MAP[sy][sx]) , recur = 1;
            }
        }
    }

    if(!recur) return;

    for(i = 0 ; i < 3 ; i++){
        for(j = 0 ; j < 3 ; j++){
            if(i == 1 && j == 1) continue;
            sy = y + dy[j] , sx = x + dx[i];
            if(inRange(sy , sx))
                if(hasOPENED(G_MAP[sy][sx]))
                    expand(sy , sx);
        }
    }

	return;
}

int inRange(int y , int x){
    return y >= 0 && x >= 0 && y < G_MAPLN && x < G_MAPWD;
}

int ArroundNum(int y , int x){
    if(!inRange(y , x)) return 0;
	int rtn = 0 , i , j , sy , sx;
    int dy[3] = {1 , 0 , -1} , dx[3] = {1 , 0 , -1};
    for(i = 0 ; i < 3 ; i++){
        for(j = 0 ; j < 3 ; j++){
            if(i == 1 && j == 1) continue; 
            sy = y + dy[j] , sx = x + dx[i];
            if(inRange(sy , sx) && G_MAP[sy][sx] % 2)
                    rtn++;
        }
    }
	return rtn;
}

bool CheckIfWin(void){
	int i , j;
	for(i = 0; i < G_MAPLN; i++)
		for(j = 0; j < G_MAPWD; j++){
			if(hasQUES(G_MAP[i][j]))
				return false;
			if(isNon(G_MAP[i][j]))
				return false;
			if(hasFLAG(G_MAP[i][j]) && hasBOMB(G_MAP[i][j]))
				return false;	
		}

	for(i = 0; i < G_MAPLN + 6 ; i++){
		printf("\033[1A");
		printf("\033[K");
	}
	for(i = 0; i < G_MAPLN; i++)
		for(j = 0; j < G_MAPWD; j++){
			if(hasNoMark(G_MAP[i][j]) && hasBOMB(G_MAP[i][j]))
				setFLAG(G_MAP[i][j]);
			if(isNon(G_MAP[i][j]))
				setOPEN(G_MAP[i][j]);
		}
	printMap(1);
	return true;
}

void GameOver(int y , int x){
	int i , j ;
	for(i = 0; i < G_MAPLN + 6 ; i++){
		printf("\033[1A");
		printf("\033[K");
	}	
	G_MAP[y][x] = 9;
	printMap(0);	
	free(G_MAP);
	puts("Game Over");
	exit(0);
}
