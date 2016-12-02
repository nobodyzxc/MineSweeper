#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/timeb.h>
#include<memory.h>
void PrintTable(int);
void DebugNum(int);
void Step(char , int , int , int* , int);
int ArroundNum(int , int , int);
int CheckIfWin(int , int);
void GameOver(int , int , int);
int **mine;
void expand(int , int , int);
int main(void){
	struct timeb timeBuf;
	int matrix , bomb , i , j , y , x ;
	char mod;
/*
	printf("Input region size:");
	scanf("%d" , &matrix);
	printf("Input land-mine number:");
	scanf("%d" , &bomb);
*/
	matrix = 10;
	bomb = 10;

	ftime(&timeBuf);
	srand(timeBuf.millitm);
	
	if(!(mine =(int**)malloc(matrix*sizeof(void*)+matrix*matrix*sizeof(int))))
		exit(1);

	int *tmp = (int *)(mine + matrix);
	for(i = 0; i != matrix; i++ , tmp += matrix)
		mine[i] = tmp;

	puts("malloc over");

	for(i = 0; i < matrix ; i++)
		for(j = 0; j < matrix ; j++)
			mine[i][j] = 0;


//	DebugNum(matrix);

	for(i = 0; i < bomb ; i++){
		while(mine[ (y = rand()%matrix) ][ (x = rand()%matrix) ] );
		mine[y][x] = 1;
	}
	puts("rand over");

//	DebugNum(matrix);
	
	setbuf(stdin, NULL);

	char Input[100];

	while(1){
		PrintTable(matrix);
		while(1){
			puts("(s/?/f) (y) (x)");
			fgets(Input , sizeof(Input) , stdin);
			if(sscanf( Input , "%c %d %d" , &mod , &y , &x) != 3){
				setbuf(stdin , NULL);
				printf("\033[1A");
				printf("\033[K");
				printf("\033[1A");
				printf("\033[K");
				continue;
			}
//			printf("get %c %d %d\n" , mod , y , x);
			setbuf(stdin,NULL);
			if((mod == 'f' || mod == 's' || mod == '?') && y >= 0 && y < matrix && x >= 0 && x < matrix)
				break;
			else{
//				printf("|%c| |%d| |%d|\n",mod , y , x);
				printf("\033[1A");
				printf("\033[K");
				printf("\033[1A");
				printf("\033[K");

			}
		}
//		puts("escape");
		Step(mod , y , x , &bomb , matrix);
//		printf("bombnum %d\n",bomb);

		if(CheckIfWin(bomb , matrix)){
			free(mine);
			puts("Find All Land Mines !");
			exit(0);
		}
		for(i = 0; i < matrix + 6 ; i++){
			printf("\033[1A");
			printf("\033[K");
		}
	}	

	return 0;
}
void DebugNum(int matrix){
	int i , j;
	for(i = 0; i < matrix; i++){
		for(j = 0; j < matrix; j++)
			printf("%d ",mine[i][j]);
		puts("");
	}
	return;
}

void PrintTable(int matrix){
	int i , j;
	printf("    ");
	for(i = 0; i < matrix; i++)
		printf("%d " , i);
	puts("");

	printf("    ");
	for(i = 0; i < matrix; i++)
		printf("- ");
	puts("");
	

	for(i = 0; i < matrix; i++){
		printf("%d | " , i);
		for(j = 0; j < matrix; j++){
			if(mine[i][j] == 0 || mine[i][j] == 1)
				printf("O ");
			else if(mine[i][j] == 6){
				if(ArroundNum(i,j,matrix) == 0)
					printf("  ");
				else
					printf("%d ",ArroundNum(i,j,matrix));
			}
			else if(mine[i][j] == 2 || mine[i][j] == 3)
				printf("? ");
			else if(mine[i][j] == 4 || mine[i][j] == 5)
				printf("F ");
			else if(mine[i][j] == 7)
				printf("* ");
			else if(mine[i][j] == 8)
				printf("X ");
			else if(mine[i][j] == 9)
				printf("# ");
			else if(mine[i][j] == 10)
				printf("Q ");
		}
		printf("| %d", i);
		puts("");
	}
	
	printf("    ");
	for(i = 0; i < matrix; i++)
		printf("- ");
	puts("");

	printf("    ");
	for(i = 0; i < matrix; i++)
		printf("%d " , i);
	puts("");

	return;
}
void Step(char mod , int y , int x , int *bomb , int matrix){
	if(mod == 's'){
		if(mine[y][x] == 0 ){
//			puts("Append");
			mine[y][x] = 6;
			expand(y , x , matrix);
		}
		else if(mine[y][x] == 1){
			GameOver(y , x , matrix);
		}
	}//(0 non) (1 present bomb) (+2 2 3 ?) 
	 //(+4 4 5 flag) (6 open);
	else if(mod == '?'){
		if(mine[y][x] == 0 || mine[y][x] == 1) mine[y][x] += 2;
		else if(mine[y][x] == 4 || mine[y][x] == 5){
			if(mine[y][x] == 5)
				(*bomb)++;
			mine[y][x] -= 2;
		}
		else if(mine[y][x] == 2 || mine[y][x] == 3)
			mine[y][x] -= 2;
	}
	else if(mod == 'f'){
		if(mine[y][x] == 0 || mine[y][x] == 1){
			if(mine[y][x] == 1)
				(*bomb)--;
			mine[y][x] += 4;
		}
		else if(mine[y][x] == 2 || mine[y][x] == 3){
			if(mine[y][x] == 3)
				(*bomb)--;
			mine[y][x] += 2;
		}
		else if(mine[y][x] == 4 || mine[y][x] == 5){
			if(mine[y][x] == 5)
				(*bomb)++;
			mine[y][x] -= 4;
		}
	}
	return;
}
void expand(int y , int x , int matrix){
	if(ArroundNum(y , x , matrix)){
		return;
	}
	int escape = 0;
	while(1){
		if(y > 0){
			if(!mine[y-1][x]){
				mine[y-1][x] = 6;
				escape = 1;
			}
			if(x > 0)
				if(!(mine[y-1][x-1])){
					mine[y-1][x-1] = 6;
					escape = 1;
				}
			if(x < matrix - 1)
				if(!(mine[y-1][x+1])){
					mine[y-1][x+1] = 6;
					escape = 1;
				}
		}

		if(x > 0)
			if(!(mine[y][x-1])){
				mine[y][x-1] = 6;
				escape = 1;
			}
		if(x < matrix - 1)
			if(!(mine[y][x+1])){
				mine[y][x+1] = 6;
				escape = 1;
			}

		if(y < matrix - 1){
			if(!(mine[y+1][x])){
				mine[y+1][x] = 6;
				escape = 1;
			}
			if(x > 0)
				if(!(mine[y+1][x-1])){
					mine[y+1][x-1] = 6;
					escape = 1;
				}
			if(x < matrix - 1)
				if(!(mine[y+1][x+1])){
					mine[y+1][x+1] = 6;
					escape = 1;
				}
		}
		if(escape) break;
		return;
	}

//	printf("expand %d %d\n" , y , x);
	if(y > 0){
		if(!(mine[y-1][x] % 6)){
			expand(y-1,x,matrix);
		}
		if(x > 0)
			if(!(mine[y-1][x-1] % 6))
				expand(y-1 , x-1 , matrix);
		if(x < matrix - 1)
			if(!(mine[y-1][x+1] % 6))
				expand(y-1 , x+1 , matrix);
	}

	if(x > 0)
		if(!(mine[y][x-1] % 6))
			expand(y , x-1 , matrix);
	if(x < matrix - 1)
		if(!(mine[y][x+1] % 6))
			expand(y , x+1 , matrix);

	if(y < matrix - 1){
		if(!(mine[y+1][x] % 6))
			expand(y+1 , x , matrix);
		if(x > 0)
			if(!(mine[y+1][x-1] % 6))
				expand(y+1 , x-1 , matrix);
		if(x < matrix - 1)
			if(!(mine[y+1][x+1] % 6))
				expand(y+1 , x+1 , matrix);
	}

	return;
}
int ArroundNum(int y , int x , int matrix){
	int num = 0;
	if(y > 0){
		if(mine[y-1][x] % 2)
			num++;
		if(x > 0)
			if(mine[y-1][x-1] % 2)
				num++;
		if(x < matrix - 1)
			if(mine[y-1][x+1] % 2)
				num++;
	}

	if(x > 0)
		if(mine[y][x-1] % 2)
			num++;
	if(x < matrix - 1)
		if(mine[y][x+1] % 2)
			num++;
	
	if(y < matrix - 1){
		if(mine[y+1][x] % 2)
			num++;
		if(x > 0)
			if(mine[y+1][x-1] % 2)
				num++;
		if(x < matrix - 1)
			if(mine[y+1][x+1] % 2)
				num++;
	}
	return num;
}
int CheckIfWin(int bomb , int matrix){
	int i , j;
	for(i = 0; i < matrix; i++)
		for(j = 0; j < matrix; j++){
			if(mine[i][j] == 2 || mine[i][j] == 3)
				return 0;
			if(mine[i][j] == 0)
				return 0;
			if(mine[i][j] == 4)
				return 0;	
		}

	for(i = 0; i < matrix + 6 ; i++){
		printf("\033[1A");
		printf("\033[K");
	}
	for(i = 0; i < matrix; i++)
		for(j = 0; j < matrix; j++){
			if(mine[i][j] == 1)
				mine[i][j] = 5;
			if(mine[i][j] == 0)
				mine[i][j] = 6;
		}
	PrintTable(matrix);
	return 1;
}
void GameOver(int y , int x , int matrix){
	int i , j ;
	for(i = 0; i < matrix + 6 ; i++){
		printf("\033[1A");
		printf("\033[K");
	}	
	for(i = 0; i< matrix; i++)
		for(j = 0; j< matrix ; j++){
			if(mine[i][j] == 1)
				mine[i][j] = 7;
			if(mine[i][j] == 4)
				mine[i][j] = 8;
			if(mine[i][j] == 0)
				mine[i][j] = 10;
		}
	mine[y][x] = 9;
	PrintTable(matrix);	
	free(mine);
	puts("Game Over");
	exit(0);
}
