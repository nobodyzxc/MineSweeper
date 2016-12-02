#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/timeb.h>
#include<memory.h>
#include<curses.h>
#include<unistd.h>
#define Y 0
#define X 0
void PrintTable(int , int , int);
void DebugNum(int);
int Step(char , int , int , int* , int , int*);
int ArroundNum(int , int , int);
int CheckIfWin(int , int , int);
void GameOver(int , int , int , int ,  int);
int **mine , y = 0 , x = 0 , debug = 0;
void expand(int , int , int);

int main(void){
	struct timeb timeBuf;
	int matrix , bomb , flag ,  i , j;

	initscr();//start curses module
	cbreak();
	noecho();
	intrflush(stdscr , FALSE);
	keypad(stdscr , TRUE);
	curs_set(0);

/*
	printf("Input region size:");
	scanf("%d" , &matrix);
	printf("Input land-mine number:");
	scanf("%d" , &bomb);
*/


	ftime(&timeBuf);
	srand(timeBuf.millitm);

	while(1){	

		matrix = 10;
		bomb = 15;
		flag = 15;

		if(!(mine =(int**)malloc(matrix*sizeof(void*)+matrix*matrix*sizeof(int))))
			endwin() , exit(1);

		int *tmp = (int *)(mine + matrix);
		for(i = 0; i != matrix; i++ , tmp += matrix)
			mine[i] = tmp;

		//	puts("malloc over");

		for(i = 0; i < matrix ; i++)
			for(j = 0; j < matrix ; j++)
				mine[i][j] = 0;


		//	DebugNum(matrix);

		for(i = 0; i < bomb ; i++){
			while(mine[ (y = rand()%matrix) ][ (x = rand()%matrix) ] );
			mine[y][x] = 1;
		}
		//	puts("rand over");

		//	DebugNum(matrix);

		//	setbuf(stdin, NULL);

		int ch; // if ch declear char will bomb
		char mod;
		y = 0 , x = 0;	

		//equal to fflush setbuf in stdio or it will start before sleep()
		nodelay(stdscr , TRUE);
		while(1){
			if(getch() == ERR)
				break;
		}
		nodelay(stdscr , FALSE);

		while(1){
			PrintTable(matrix , bomb , flag);
			mvprintw(Y + y + 1, X + 2 *(x+1) + 1 , "<");//?
			ch = getch();
			mvprintw(Y + y + 1 , X + 2 *(x+1) + 1 , " ");//?
			mod = 'N';//present no mod
			switch(ch){
				case KEY_UP:
					if(y > 0)
						y--;
					break;
				case KEY_DOWN:
					if(y < matrix - 1)
						y++;
					break;
				case KEY_LEFT:
					if(x > 0)
						x--;
					break;
				case KEY_RIGHT:
					if(x < matrix - 1)
						x++;
					break;
				case 32:
				case '\n':
				case '\r':
					mod = 's';
					break;
				case 102:
				case 70:
					mod = 'f';		
					break;
				case 63:
					mod = '?';
					break;
				case 'd':
				case 'D':
					debug = 1 - debug;
					break;
				case 27:
					endwin();
					exit(0);
					break;
			}
			if(Step(mod , y , x , &bomb , matrix , &flag))
				break;
			if(CheckIfWin(bomb , matrix , flag)){
				free(mine);
				mvprintw(Y + matrix +2 , X + 5 , "Find All Land Mines !");
				mvprintw(Y + matrix + 3 , X + 5 , "Press any botton to continue ");
				getch();
				for(i = 0; i< 5 ; i++){
					addch('.');
					refresh();
					sleep(1);
				}
				setbuf(stdin , NULL);
				mvprintw(Y + matrix +2 , X + 5 , "                     ");
				mvprintw(Y + matrix + 3 , X + 5 , "                                    ");
				break;
			}
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

void PrintTable(int matrix , int bomb , int flag){
	int i , j;

	mvprintw(Y + 1 , X + (matrix+1)*2 + 5 , "%2d flag(s) left" , flag);
	if(debug == 1)
	mvprintw(Y + 2 , X + (matrix+1)*2 + 8 , "%2d bomb left" , bomb) ;
	else
	mvprintw(Y + 2 , X + (matrix+1)*2 + 8 , "            " , bomb) ;
	for(i = 0; i < matrix; i++)
		mvprintw(Y + 0 , X + (i + 1)*2 , "- ");
	
	for(i = 0; i < matrix; i++){
		mvprintw(Y + i + 1 , X + 0 , "|");
		for(j = 0; j < matrix; j++){
			if(mine[i][j] == 0 || mine[i][j] == 1)
				mvprintw(Y + i + 1 , X + 2 *(j+1) , "O");
			else if(mine[i][j] == 2 || mine[i][j] == 3)
				mvprintw(Y + i + 1, X + 2 *(j+1) , "?");
			else if(mine[i][j] == 4 || mine[i][j] == 5)
				mvprintw(Y + i + 1, X + 2 *(j+1) , "F");
			else if(mine[i][j] == 6){
				if(ArroundNum(i,j,matrix) == 0)
					mvprintw(Y + i + 1 , X + 2 *(j+1) , " ");
				else
					mvprintw(Y + i + 1, X + 2 *(j+1) ,"%d",ArroundNum(i,j,matrix));
			}
			else if(mine[i][j] == 7)
				mvprintw(Y + i + 1, X + 2 *(j+1) , "*");
			else if(mine[i][j] == 8)
				mvprintw(Y + i + 1, X + 2 *(j+1) , "X");		
			else if(mine[i][j] == 9)
				mvprintw(Y + i + 1, X + 2 *(j+1) , "#");
			else if(mine[i][j] == 10)
				mvprintw(Y + i + 1, X + 2 *(j+1) , "Q");
		}
		mvprintw(Y + i + 1 , X + 2*(matrix+1) , "|");
	}
	
	for(i = 0; i < matrix; i++)
		mvprintw(Y + matrix + 1 , X + (i+1)*2 , "- ");

	return;
}
int Step(char mod , int y , int x , int *bomb , int matrix , int *flag){
	if(mod == 's'){
		if(mine[y][x] == 0 ){
			mine[y][x] = 6;
			expand(y , x , matrix);
		}
		else if(mine[y][x] == 1){
			GameOver(y , x , matrix , *bomb , *flag);
			return 1;
		}
	}//(0 non) (1 present bomb) (+2 2 3 ?) 
	 //(+4 4 5 flag) (6 open); // 7 9 11
	else if(mod == '?'){
		if(mine[y][x] == 0 || mine[y][x] == 1) mine[y][x] += 2;
		else if(mine[y][x] == 4 || mine[y][x] == 5){
			if(mine[y][x] == 5)
				(*bomb)++;
			(*flag)++;
			mine[y][x] -= 2;
		}
		else if(mine[y][x] == 2 || mine[y][x] == 3)
			mine[y][x] -= 2;
	}
	else if(mod == 'f'){
		if((mine[y][x] == 0 || mine[y][x] == 1) && (*flag) > 0){
			if(mine[y][x] == 1)
				(*bomb)--;
			(*flag)--;
			mine[y][x] += 4;
		}
		else if((mine[y][x] == 2 || mine[y][x] == 3) && (*flag) > 0){
			if(mine[y][x] == 3)
				(*bomb)--;
			(*flag)--;
			mine[y][x] += 2;
		}
		else if(mine[y][x] == 4 || mine[y][x] == 5){
			if(mine[y][x] == 5)
				(*bomb)++;
			(*flag)++;
			mine[y][x] -= 4;
		}
	}
	return 0;
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
int CheckIfWin(int bomb , int matrix , int flag){
	int i , j;
	for(i = 0; i < matrix; i++)
		for(j = 0; j < matrix; j++){
			if(mine[i][j] == 2 || mine[i][j] == 3)
				return 0;
			if(mine[i][j] == 0)//remove == 0
				return 0;
			if(mine[i][j] == 4)
				return 0;
		}

	for(i = 0; i < matrix; i++)
		for(j = 0; j < matrix; j++){
			if(mine[i][j] == 1)
				mine[i][j] = 5;
			if(mine[i][j] == 0)
				mine[i][j] = 6;
		}
	flag = 0;
	bomb = 0;
	PrintTable(matrix , bomb , flag);
	return 1;
}
void GameOver(int y , int x , int matrix , int bomb , int flag){
	int i , j ;
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
	PrintTable(matrix , bomb  , flag);	
	free(mine);
	mvprintw(Y + matrix + 2, X + 5 , "Game Over");
	mvprintw(Y + matrix + 3 , X + 5 , "Press any botton to continue ");
	getch();
	for(i = 0; i< 5 ; i++){
		addch('.');
		refresh();
		sleep(1);
	}
	setbuf(stdin , NULL);
	mvprintw(Y + matrix + 2, X + 5 , "              ");
	mvprintw(Y + matrix + 3 , X + 5 , "                                     ");
	endwin();
}
