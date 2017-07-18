#include<stdio.h>
#include<windows.h>
#include<Winuser.h>
#include<Windows.h>
#include<process.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<time.h>
#include<memory.h>
#define Y 16
#define X 30
#define BoxLength 35
#define MOX 175 //mouse location
#define MOY 119
#define POX 157 //pixel location
#define POY 59
#define FLAGNUM 99
struct notation{
	 float num;
	 int exp;
};
struct point_{
    int y;
    int x;
};
int direct(int);
int direct_single(int , int);
void CLKARD(int , int , char *);
int reasoning(int i , int j);
int dfs_reasoning(void);
int suppose(int);
int resuppose(void);
void dfs(int , int);
int check_if_right(int , int);
void CLKRAND(void);
int Arround(int , int , char* , int matrix[][X]);
int if_in_range(int);
int if_zero(int , int , char*);
void print_array(char matrix_name[]);
void analyze_screen(void);
int analyze_single(int , int);
void MouseSetting(void);
void Click(int , int , char *);
void dp(int , int, int detect[][X]);
void Gaming(void);
int check_if_win(void);
int All_count(int , int matrix[][X]);
void open_window(char *);
int neighbor_num(int , int);
struct point_* rtnpt(int , int , int);
int need_flag(void);

int game_plat_possible;
int left_mine_unknown;
int out_to_rand = 0;
int add_speed = 1;

struct point_* rtnpt(int y , int x  , int which){
    struct point_ *point;

    int all_stop;
    int count = 0;
    int i , j ;

    all_stop = neighbor_num(y , x);

    for(i = -1 ; i <= 1; i++){
        for(j = -1 ; j <= 1 ; j++){
            if(i == 0 && j == 0) continue;
            if(y + i < 0 || y + i >= Y || x + j < 0 || x + j >= X) continue;
            if(which == all_stop){
                return NULL;
            }
            if(count == which){
                point = (struct point_*)malloc(sizeof(struct point_));
                point->y = y + i;
                point->x = x + j;
                return point;
            }
            count++;
        }
    }
}

void thread_open_game(void *p){
	system("C:\\\"Program Files\"\\\"Microsoft Games\"\\Minesweeper\\minesweeper.exe");
	return;
}

POINT pt;
HWND wndPane  , wndPaneParent , wndGameLost , wndGameWon , wndconsole;
HDC hdc ;

MOUSEINPUT mDown , mUp , nDown , nUp;
INPUT inputMouseDown_L , inputMouseUp_L ,
		inputMouseDown_R , inputMouseUp_R;

int array[Y][X] = {0} ;
int reason_array[Y][X];
int button[Y][X] = {0};
int border[Y][X] = {0};
struct notation dfs_count[Y][X] ;//= { [0 ... Y][0 ... X] = {
								//							.num = 0  .exp = 0 }  };
float flags = FLAGNUM;
float dfs_flags = FLAGNUM;
int WIN_NUM = 0;
int LOSE_NUM = 0;


void open_window(char *wnd){
    static char nowwin = '?';

	if(!strcmp(wnd , "minesweeper")){
		if(nowwin != 'm'){
            ShowWindow(wndPaneParent , 2);
		    ShowWindow(wndPaneParent , 3);
		    nowwin = 'm';
		}
	}
	else if(!strcmp(wnd , "console")){
		if(nowwin != 'c'){
            ShowWindow(wndconsole , 2);
		    ShowWindow(wndconsole , 3);
		    nowwin = 'c';
		}
	}
	else{
		puts(wnd);
		puts("no such window");
		exit(0);
	}
	Sleep(1000);
	return;
}

int main(void){

	Sleep(3000);
	srand(time(NULL));
	int times;
	wndconsole = GetConsoleWindow();
	wndPaneParent = FindWindow(NULL , "Minesweeper");

	if(wndPaneParent == NULL) {
		puts("No such windows\nopen game !!");

		char Data[10];
		_beginthread(thread_open_game , 0 , (void *)Data);
		Sleep(3000);
		wndPaneParent = FindWindow(NULL , "Minesweeper");
		puts("finish get parent");
		if(wndPaneParent == NULL) puts("can't open minesweeper") , exit(0);
	}


	wndPane = GetWindow( wndPaneParent , 5);


	if(wndPane == NULL){
		puts("can not get child wnd of minesweeper !");
		exit(0);
	}
	else{
		puts("get child successfully");
	}

	hdc = GetDC(wndPane);
	if(hdc == NULL) {
		puts("No such hdc");
		return 0;
	}
	open_window("minesweeper");

	puts("open over");

	MouseSetting();


	for(times = 0 ; times < 2 ; times++){

		Click(999 , 999 , "left");
		int i , j , times;
		int r , g , b;
		for(i = 0 ; i< Y ; i++){
			for(j = 0 ; j<X ; j++){
				array[i][j] = 0;
				reason_array[i][j] = 0;
				button[i][j] = 0;
				border[i][j] = 0;
			}
		}


		if(check_if_win()) continue;



		flags = (float) (FLAGNUM - All_count(9 , array));

		analyze_screen();
		analyze_screen();
		if(1)
			Gaming();
		if(0){
			resuppose();
			print_array("dfs_count") ;
		}

		print_array("array");

		puts("over");
	}

	printf("TOTAL GAME NUMBER = %d\n          WIN_NUM = %d\n         LOSE_NUM = %d \n" , times , WIN_NUM , LOSE_NUM);

	return 0;
}
void Gaming(void){
	int i , j , modify = 0;

	puts("gaming");

	while(1){

		int pass = 1;
		if(All_count(0 , array) == Y * X && pass){
		    puts("all zero == Y*X");
            open_window("console");
            pass = 0;
            CLKRAND();
        	analyze_screen();
            continue;
        }

		if(direct(0)) continue;

		if(check_if_win()) return ;

		if(suppose(9)){
			puts("suppose 9 success !!");
			continue;
		}

		if(suppose(10)){
			puts("suppose 10 success !!");
			continue;
		}


		if(check_if_win()) return ;

		if(All_count(0 , array) == 0) break;



        resuppose();
		puts("game ooo");

	}
	return ;
}

int direct(int if_analyze){

	int modify = 0;
	int i, j ;

	if(if_analyze){
	   analyze_screen();
	   print_array("array");
	}


	for( i = 0 ; i < Y ; i ++){
		for( j = 0 ; j < X ; j++ ){
            if(direct_single(i , j)) modify = 1;
		}
	}

	return modify;
}

int direct_single(int i , int j){
    int modify = 0;
	if(button[i][j] == 0){
		if(array[i][j] > 0 && array[i][j] < 9){

			int arround_space_num = Arround(i , j , "space" , array);
			int arround_flag_num = 	Arround(i , j , "flag" , array);


			if(arround_flag_num == array[i][j] && arround_space_num != 0){
				printf("%d %d arround flag = %d , space = %d , self = %d\n", i , j , arround_flag_num , arround_space_num , array[i][j]);
				CLKARD(i , j ,"space");

				modify = 1;
			}
			if(arround_space_num == array[i][j] - arround_flag_num && arround_space_num != 0 && array[i][j] != 8 && array[i][j] != 7){
				CLKARD(i , j , "flag");
				modify = 1;
			}
			if(arround_space_num == 0 && array[i][j] == arround_flag_num){
				button[i][j] = 1;
			}
		}
	}

	return modify;
}

void CLKARD(int y , int x , char *mod){
	int revise;
	char click_mod[20];
	printf("click %d %d , %s\n" , y , x, mod);
	if(!strcmp(mod , "flag")){
		strcpy(click_mod , "right");
		revise = 9;
	}
	else if(!strcmp(mod , "space")){
		Click(x , y , "both");
		printf("%d %d both\n" , y , x);
		return ;
	}

    struct point_ * point;
    int which_pt = 0;
    while(point = rtnpt( y , x , which_pt++)){
        if(!array[point->y][point->x]){
            Click(point->x , point->y , click_mod);
            Sleep(50);
        }
        free(point);
    }

	return ;
}

int suppose(int what){
	int i , j , k , l;
	int tem_num;
	int result = 0;


	for(k = 0; k < Y ; k++){
		for(l = 0 ; l < X ; l++){
			reason_array[k][l] = array[k][l];
		}
	}

	puts("\n\n              suppose            !!!!!             !!!!!     ");
	print_array("reason_array");
	puts("              suppose            !!!!!             !!!!!     \n");

	memset(border , 0 , sizeof(border));
    for(i = 0 ; i < Y ; i++)
        for(j = 0 ; j < X ; j++){
            int tem_num = Arround( i , j ,"space" , array) + Arround(i , j , "clear" , array) + Arround(i , j , "flag" , array);

			if(array[i][j] == 0 && tem_num < neighbor_num(i , j) && tem_num > 0)
			    border[i][j] = 1;
			else
				border[i][j] = 0;
		}

	for(i = 0; i < Y ; i++){
		for(j = 0 ; j < X ; j++){

			if(reason_array[i][j] == 0 ){

                if(border[i][j]){
			  		// if there is any number arround i j , and there is any space arround i j

					reason_array[i][j] = what; // suppose !!!

					if(reasoning(i , j)){
						printf("%d %d suppose success !! click it and next !!\n", i , j);

						if(what == 9){
							Click(j , i , "left");
						}
						else if(what == 10){
							Click(j , i , "right");
						}

						result = 1;

						while(1){// may be wrong !! direct(1) origion
							if(direct(0) == 1) continue;
							else{// ?? if analyze_screen??
								break;
							}
						}

						for(k = 0; k < Y ; k++){
							for(l = 0 ; l < X ; l++){
								reason_array[k][l] = array[k][l];
							}
						}


					}
					else{
						for(k = 0; k < Y ; k++){
							for(l = 0 ; l < X ; l++){
								reason_array[k][l] = array[k][l];
							}
						}
					}
				}
			}

		}
	}
	printf("X = %d Y= %d" , X , Y);
	puts("suppose return\n\n");

	return result;
}

int reasoning(int y , int x){//input a space with num arround !

    int which_pt = 0;
    struct point_ * point;
    while(point = rtnpt( y , x , which_pt++)){
        if(if_in_range(reason_array[point->y][point->x]) && reason_array[point->y][point->x] == Arround(point->y , point->x , "flag" , reason_array))
            if(if_zero(point->y , point->x , "open")) return 1;

        if(if_in_range(reason_array[point->y][point->x]) && reason_array[point->y][point->x] - Arround(point->y , point->x , "flag" , reason_array) == Arround(point->y , point->x , "space" , reason_array))
			if(if_zero(point->y , point->x , "flag")) return 1;

		if(if_in_range(reason_array[point->y][point->x]) && Arround(point->y , point->x , "space" , reason_array) == 0 &&
											reason_array[point->y][point->x] != Arround(point->y , point->x , "flag" , reason_array))
			return 1;

        free(point);
    }

	return 0;
}
int if_zero(int y , int x , char *mod){


	int destin;
	int queueX[8] = {0} ;
	int queueY[8] = {0} ;
	int i = 0;
	int j = 0;

	if(!strcmp(mod , "open")) destin = 10;
	else if(!strcmp(mod , "flag")) destin = 9;
	else puts("wrong") , exit(0);

    int which_pt = 0;
    struct point_ * point;
    while(point = rtnpt( y , x , which_pt++)){
        if(reason_array[point->y][point->x] == 0){
            reason_array[point->y][point->x] = destin;
		    queueY[i] = point->y;
		    queueX[i] = point->x;
		    i++;
		}
        free(point);
    }

	if(y == 6 || y == 5){

	puts("OPEN NING");
	print_array("reason_array");

	}

	for(j = 0 ; j < i ; j++){
		if(reasoning(queueY[j] , queueX[j]))
			return 1;
	}

	return 0;
}

int if_in_range(int range){
	if(range > 0 && range < 7)// origin 9 because 7 and 8 can't be sure
		return 1;
	else
		return 0;
}

int neighbor_num(int y , int x){
	if(y == 0 || y == Y-1){
		if(x == 0 || x == X-1){
			return 3;
		}
		else{
			return 5;
		}
	}
	else{
		if(x == 0 || x == X-1){
			return 5;
		}
		else{
			return 8;
		}
	}
}

int resuppose(void){

	int i , j , k , l;
	int tem_num;
	int exist_unknown = 0;
	int result = 0;

	analyze_screen();
	open_window("console");
	Sleep(1000);

	game_plat_possible = 0;
	left_mine_unknown = 0;

	print_array("array");
	flags = (float)(FLAGNUM - All_count(9 , array));
	printf("flags == %f ,FLAGNUM is %d ,  All_count 9 is %d \n" , flags , FLAGNUM , All_count(9 , array));
	dfs_flags = flags;
	printf("dfs_flags = %f\n" , dfs_flags);

	//initial dfs_count

	for(k = 0 ; k < Y ; k++)
		for(l = 0 ; l < X ; l++ ){
			dfs_count[k][l].num = 0;
			dfs_count[k][l].exp	= 0;
		}

	//initial reason_array & dfs_count

	for(k = 0; k < Y ; k++){
		for(l = 0 ; l < X ; l++){

			reason_array[k][l] = array[k][l];

			if(array[k][l] != 0){
				dfs_count[k][l].num = -1;
			}
			else if(neighbor_num(k , l) - Arround(k , l , "space" , array) - Arround(k , l , "flag" , array) == 0 ){
				dfs_count[k][l].num = 0;// == -2
				exist_unknown = 1;
			}
			else{
				dfs_count[k][l].num = 0;
			}
		}
	}

	puts("\n\n              resuppose   start         !!!!!             !!!!!     ");
	print_array("reason_array");
	puts("              resuppose            !!!!!             !!!!!     \n");
	print_array("dfs_count");
	puts("");
	print_array("border");
	printf("dfs_flags == %f\n" , dfs_flags);
	getchar();

    out_to_rand = 0;
    //something strange, so i make border again

    if(array[i][j] == 0 && tem_num < neighbor_num(i , j) && tem_num > 0)// **
		border[i][j] = 1;
	else
		border[i][j] = 0;

	dfs(0 , 0);
	puts("out dfs");
	if(out_to_rand == 10000){
		CLKRAND();
		return 0;
	}
	for(i = 0 ; i < Y ; i++)
		for(j = 0 ; j < X ; j++){
			if(dfs_count[i][j].num != -1)
			while(1){
				if((dfs_count[i][j].num >= 1 && dfs_count[i][j].num < 10) || dfs_count[i][j].num == 0)
					break;
				else if(dfs_count[i][j].num < 1){
					dfs_count[i][j].num *= 10;
					dfs_count[i][j].exp -= 1;
				}
				else if(dfs_count[i][j].num >= 10){
					dfs_count[i][j].num /= 10;
					dfs_count[i][j].exp += 1;
				}
				if(dfs_count[i][j].num < 0){
					printf("< 0 ERr");
					exit(0);
				}
			}

		}

	printf("X = %d Y= %d\n" , X , Y);
	puts("resuppose return\n\n");

	print_array("dfs_count") ;
	printf("All game plat sum == %d\n" , game_plat_possible);
	if(game_plat_possible == 0) exit(0);
	printf("left mine unknow %s\n" , left_mine_unknown ? "TRUE" : "FALSE");
    Sleep(2000);
	getchar();




	if(exist_unknown == 1 && left_mine_unknown == 0){
    	puts("start click which blocks for sure");
		getchar();
		Sleep(2500);

    	open_window("minesweeper");
    	Sleep(1000);
		result = 1;
		for(k = 0; k < Y ; k++){
			for(l = 0 ; l < X ; l++){
				if(neighbor_num(k , l) - Arround(k , l , "space" , array) - Arround(k , l , "flag" , array) == 0){//dfs_count[k][l] == -2)
					Click(l , k , "left");
				}
			}
		}
	}

	int bestX = 0 , bestY = 0 , bestchosen = -3;
	int best_total = 1;

	for(k = 0 ; k < Y ; k++){
		for(l = 0 ; l < X ; l++){
			if(dfs_count[k][l].num == 0 && dfs_count[k][l].exp == 0 && neighbor_num(k , l) - Arround(k , l , "space" , array) - Arround(k , l , "flag" , array) != 0){
				if(result == 0){
				    puts("click bolcks for sure");
					getchar();
				    open_window("minesweeper");
				    Sleep(1000);
				}
                Click(l , k , "left");
				result = 1;
			}
			if(dfs_count[k][l].num >= 0){
				if(bestchosen == -3){
					bestY = k;
					bestX = l;
					bestchosen = 0; // ??
				}
				else{
					if(left_mine_unknown == 0 || left_mine_unknown == 1){
						if(dfs_count[bestY][bestX].exp > dfs_count[k][l].exp){
							bestY = k;
							bestX = l;
							best_total = 1;
						}
						else if(dfs_count[bestY][bestX].exp == dfs_count[k][l].exp){

							if(dfs_count[bestY][bestX].num > dfs_count[k][l].num){
								bestY = k;
								bestX = l;
								best_total = 1;
							}
							else if(dfs_count[bestY][bestX].num == dfs_count[k][l].num){
								best_total++;
							}
						}
					}

				}
			}

		}
	}


    open_window("console");
	Sleep(1000);

	if(result == 0){

		if(game_plat_possible == 1) {
       		puts("only one plat , will rand!!");
        	Sleep(2500);
			CLKRAND() ;
			return 0;
		}
		else{


			printf("best total is %d\n" , best_total);
	    	int rand_num = rand() % best_total ;

			printf("chose NO.%d pt(start from 0)\n" , rand_num);

			for(k = 0 ; k < Y ; k++){

				for(l = 0 ; l < X ; l++){
					if((dfs_count[k][l].exp == dfs_count[bestY][bestX].exp) && (dfs_count[k][l].num == dfs_count[bestY][bestX].num)){

						printf("%d %d same to the best\n" , k , l);
						if(rand_num == 0){
							break;
						}
						else{
							rand_num--;
						}

					}
				}

				if(rand_num == 0 && l < X){ // if i doesn't add l < X, it will be wrong , it's so important!!
					break;
				}

			}

	    	printf("will click bestY = %d , bestX = %d\n" , k , l);
	    	printf("num = %1.1f , exp = %d\n" , dfs_count[k][l].num , dfs_count[k][l].exp);
	    	printf("is ( %d )\n" , reason_array[k][l]);
	    	puts("Enter to continue...");
	  		getchar();
    		open_window("minesweeper");
        	Click(l , k  ,"left");
        }
	}
	else{
	   puts("plat have change, will continue game");
	   Sleep(2500);
	   open_window("minesweeper");
	}

	game_plat_possible = 0;
	left_mine_unknown = 0;

	return result;
}


struct notation C(int P , int C){// , int y , int x){
	int i ;
	int tem , fac = 1;
	struct notation Pno;
	struct notation Cno;

	Pno.num = 1;
	Cno.num = 1;
	Pno.exp = 0;
	Cno.exp = 0;

	if(C > P-C)
		tem = C;
	else
		tem = P-C;

	for(i = P ; i > tem ; i--){
		Pno.num *= i;
		Cno.num *= fac;
		if(Pno.num > 1000){
			Pno.exp += 3;
			Pno.num /= 1000;
		}
		if(Cno.num > 1000){
			Cno.exp += 3;
			Cno.num /= 1000;
		}
		fac++;
	}

	Pno.num /= Cno.num;
	Pno.exp -= Cno.exp;

	while(1){
		if(Pno.num >= 1 && Pno.num < 10)
			break;
		else if(Pno.num < 1){
			Pno.num *= 10;
			Pno.exp -= 1;
		}
		else if(Pno.num >= 10){
			Pno.num /= 10;
			Pno.exp += 1;
		}
		else if(Pno.num < 0){
			printf("Pno.num < 0 ERr");
			exit(0);
		}
	}


	return Pno;
}

void addC(struct notation *beadd , struct notation add){
	int i;

	if((*beadd).exp == add.exp){
		(*beadd).num += add.num;
	}
	else if((*beadd).exp > add.exp){
		for(i = 0 ; i < (*beadd).exp - add.exp; i++){
			add.num *= 0.1;
		}
		(*beadd).num += add.num;
	}
	else if((*beadd).exp < add.exp){
		for(i = 0 ; i < add.exp - (*beadd).exp; i++){
			(*beadd).num *= 0.1;
		}
		(*beadd).num += add.num;
		(*beadd).exp = add.exp;
	}

	while(1){
		if((*beadd).num >= 1 && (*beadd).num < 10)
			break;
		else if((*beadd).num < 1){
			(*beadd).num *= 10;
			(*beadd).exp -= 1;
		}
		else if((*beadd).num >= 10){
			(*beadd).num /= 10;
			(*beadd).exp += 1;
		}
		else if((*beadd).num < 0){
			printf("beadd < 0 ERr");
			exit(0);
		}
	}


	return;
}
void dfs(int y , int x){

	if(reason_array[15][26] == 10){
		print_array("reason_array");
		printf(" 1 y %d  x %d\n" , y , x);
	}
    out_to_rand ++;

    if(out_to_rand % 10000 == 0){
        printf("out of %d \n" , out_to_rand);
        if(out_to_rand == 10000) return;
        Sleep(5000);
    }

	int i , j , k , l , tem_num;

	for(i = 0 ; i < Y ; i++){
		for(j = 0 ; j < X ; j++){



			if(y != 0 && x != 0 && ( (i < y ) || (i == y && j <= x))) continue;


			printf("suppose into %d %d\n" , i , j);

			if(reason_array[i][j] == 0 ){

                if(border[i][j]){
			  		// if there is any number arround i j , "and there is any space arround i j"??
			  		print_array("reason_array");
					printf("%d %d\n" , i , j );


					if(All_count(0 , reason_array) - 1 >= dfs_flags){

						reason_array[i][j] = 10; // suppose !!!

						int result = check_if_right(i , j);

						if(result == 0){
							dfs(i , j);
						}
						else if(result == 1){
							puts("AAAAA???");
							out_to_rand = 10000;
							return;
						}
						else{

						}
					}
					else{// fault

					}



					puts("into flags");

					if(dfs_flags >= 1){

						printf("%d %d supplse as flags\n" , i , j);
						reason_array[i][j] = 9;
						dfs_flags--;



						int result;
						result = check_if_right(i , j);


						if(result == 0){
							dfs(i , j);
							dfs_flags++;
						}
						else{
							if(dfs_flags == 0 && (result == 1 || result == 0) ){
								printf("\n%d %d  -- = falgs = %f \n" , i , j , dfs_flags );
								puts("\n\n may right vvv\n\n");
								puts("-----------flags no left !!!!-------------");
								print_array("reason_array");
								for(k = 0 ; k < Y ; k++)
									for(l = 0 ; l < X ; l++){
										if(dfs_count[k][l].num != -1)
											if(reason_array[k][l] == 9)
												dfs_count[k][l].num++;
									}

								dfs_flags++;
								game_plat_possible ++;
							}
							else{// == -1 || flags != 0


								if(result == 1 || result == 0){

									int a , b;
									int backup[Y][X];

									//backup becuase the op below
									for(a = 0 ; a != Y ; a++)
										for( b = 0 ; b != X ; b++)
											backup[a][b] = reason_array[a][b];

									// transfer the 0 on border to 10

									for(a = 0 ; a != Y ; a++)
										for( b = 0 ; b != X ; b++){
											if(reason_array[a][b] != 0){
												if(Arround(a , b , "flag" , reason_array) == reason_array[a][b] && Arround(a , b , "space" , reason_array) != 0){
													struct point_ * point;
    												int which_pt = 0;
    												while(point = rtnpt( a , b , which_pt++)){
        												if(!reason_array[point->y][point->x]){
            												reason_array[point->y][point->x] = 10;
        												}
        												free(point);
    												}
												}
											}
										}


								// start calclulate Combanation
									struct notation temC;
									temC = C(All_count(0 , reason_array) , (int)dfs_flags);
									printf("A 0 %d ,flags %d\n" ,All_count(0 , reason_array) , (int)dfs_flags);
									printf("add %f %d\n" , temC.num , temC.exp);
									for(k = 0 ; k < Y ; k++)
										for(l = 0 ; l < X ; l++){
											if(dfs_count[k][l].num >= 0)
												if(reason_array[k][l] == 9){
													addC( &dfs_count[k][l] , temC);
												}
												else if(neighbor_num(k , l) - Arround(k , l , "space" , array) - Arround(k , l , "flag" , array) == 0 && reason_array[k][l] == 0){
													if((k == 14 || k == 15)&&(l >= 24 && l <= 26))printf("=> %d %d \n" , k , l);
													addC( &dfs_count[k][l] , temC);
												}
										}
										printf("----------flags left %f---------\n\n\n" , dfs_flags);
										left_mine_unknown = 1; // here !!
										print_array("reason_array");
										puts("^  ^^  ^ final !!!\n\n\n");
										game_plat_possible ++;

									//recover reason_array
									for(a = 0 ; a != Y ; a++)
										for( b = 0 ; b != X ; b++)
											reason_array[a][b] = backup[a][b];

								}
								else{
										puts("ERR3.1");
								}

								dfs_flags++;
							}
						}

					}
					else{
						if(dfs_flags != 0){
							printf("flags = 0 < 0 %f  ??????\n" , dfs_flags);
							Sleep(20000);
							exit(0);
						}
					}

					reason_array[i][j] = 0;

					return;
				}
			}
		}
	}

	int a , b;
	int backup[Y][X];
	//backup becuase the op below
	for(a = 0 ; a != Y ; a++)
		for( b = 0 ; b != X ; b++)
			backup[a][b] = reason_array[a][b];

	// transfer the 0 on border to 10

	for(a = 0 ; a != Y ; a++)
		for( b = 0 ; b != X ; b++){
			if(reason_array[a][b] != 0){
				if(Arround(a , b , "flag" , reason_array) == reason_array[a][b] && Arround(a , b , "space" , reason_array) != 0){
					struct point_ * point;
					int which_pt = 0;
					while(point = rtnpt( a , b , which_pt++)){
						if(!reason_array[point->y][point->x]){
							reason_array[point->y][point->x] = 10;
						}
						free(point);
					}
				}
			}
		}

	// start calclulate Combanation
	struct notation temC;
	temC = C(All_count(0 , reason_array) , (int)dfs_flags);
	printf("A 0 %d ,flags %d\n" ,All_count(0 , reason_array) , (int)dfs_flags);
	printf("add %f %d\n" , temC.num , temC.exp);
	for(k = 0 ; k < Y ; k++)
		for(l = 0 ; l < X ; l++){
			if(dfs_count[k][l].num >= 0)
				if(reason_array[k][l] == 9){
					addC( &dfs_count[k][l] , temC);
				}
				else if(neighbor_num(k , l) - Arround(k , l , "space" , array) - Arround(k , l , "flag" , array) == 0 && reason_array[k][l] == 0){
					if((k == 14 || k == 15)&&(l >= 24 && l <= 26))printf("=> %d %d \n" , k , l);
					addC( &dfs_count[k][l] , temC);
				}
		}
	printf("----------flags left %f---------\n\n\n" , dfs_flags);
	left_mine_unknown = 1; // here !!
	print_array("reason_array");
	puts("^      ^ final !!!\n\n\n");
	game_plat_possible ++;

	//recover reason_array
	for(a = 0 ; a != Y ; a++)
		for( b = 0 ; b != X ; b++)
			reason_array[a][b] = backup[a][b];


	return ;
}

int check_if_right(int y , int x){

	int value = 1;
	int tem;
	int pem;
	int which_pt = 0;
    struct point_ * point;

    if(y != -1){
        while(point = rtnpt( y , x , which_pt++)){
            if(reason_array[point->y][point->x] > 0 && reason_array[point->y][point->x]< 9){
			    tem = Arround(point->y , point->x , "flag" , reason_array);
			    pem = Arround(point->y , point->x , "space" , reason_array);
			    if(reason_array[point->y][point->x] < tem){
	                printf("%d %d is %d while F is %d\n" , point->y , point->x , reason_array[point->y][point->x] , tem);
					print_array("reason_array");

					return -1;
			    }
                else if(reason_array[point->y][point->x] > tem && pem == 0){
				    printf("%d %d is %d while ArroundF is %d space is %d\n" , point->y , point->x , reason_array[point->y][point->x] ,  tem , pem);
					print_array("reason_array");

					return -1;
			    }
		    }
            free(point);
        }
        //reason_array[point->y][point->x] > tem && pem > 0
    	if(need_flag()){
			if(dfs_flags  >= 1){
				value = 0;
		    }
		    else{
		    	puts("need flags while no space left");
		    	print_array("reason_array");
			    return -1;
		    }
		}
    }
	else{

    	int i , j ;
	    for(i = 0 ; i < Y; i++){
			for(j = 0 ; j < X ; j++){
				if(reason_array[i][j] > 0 && reason_array[i][j]< 9){
					tem = Arround(i , j , "flag" , reason_array);
					pem = Arround(i , j , "space" , reason_array);
					if(reason_array[i][j] < tem){
						printf("%d %d is %d while F is %d\n" , i , j , reason_array[i][j] , tem);
                	    return -1;
					}
					else if(reason_array[i][j] > tem && pem == 0){
						printf("%d %d is %d while ArroundF is %d space is %d\n" , i , j , reason_array[i][j] ,  tem , pem);
						return -1;
					}
					else if(reason_array[i][j] > tem && pem > 0){
						if(dfs_flags  >= 1){
							value = 0;
						}
						else{
							return -1;
						}
					}
				}
			}
		}
	}
	return value;
}
int need_flag(void){
	int i , j , tem , pem;
	for(i = 0 ; i < Y ; i++)
		for(j = 0 ; j < X ; j++){
			if(reason_array[i][j] > 0 && reason_array[i][j] < 7){// 7 8 ignore !!
				tem = Arround(i , j , "flag" , reason_array);
				pem = Arround(i , j , "space" , reason_array);
				if(reason_array[i][j] > tem && pem > 0){
					return 1;
				}
			}
		}
	return 0;
}
void CLKRAND(void){
	int arround_tem;
	int x , y ;
	int i , j ;
	float num_of_unknown = 0 , arround_to_add;
	float probability[Y][X] = {0} , max = 0;

	puts("\nstart clkrand !!\n");


	for(i = 0; i < Y ; i++){
		for(j = 0; j < X; j++){
			if(array[i][j] == 0) num_of_unknown++;
		}
	}


	if(num_of_unknown == 0){
		puts("num_of_unknown == 0");
		exit(0);
	}

	struct point_ * point;
    int which_pt = 0;
	for(i = 0; i < Y ; i++){
		for(j = 0; j < X; j++){
			if(array[i][j] != 0 && array[i][j] != 9 &&
								   array[i][j] != 10){

				arround_tem = Arround(i , j , "space" , array);

				if(arround_tem == 0){
					continue;
				}
				else{
					arround_to_add = ((float)(array[i][j] - Arround(i , j , "flag" , array)) / (float)arround_tem);
				}
                which_pt = 0;
                while(point = rtnpt( i , j , which_pt++)){
					if(!array[point->y][point->x]){
                        if(arround_tem)
						    probability[point->y][point->x] += (1 - probability[point->y][point->x]) * arround_to_add;
					}
                    free(point);
                }

			}
		}
	}

	for(i = 0; i < Y ; i++){
		for(j = 0; j < X ; j++){
			if(array[i][j] == 0){
				if(num_of_unknown != 0)
					if(probability[i][j] == 0)
						probability[i][j] = flags / num_of_unknown;
			}
		}
	}


	for(i = 0; i < Y ; i++){
		for(j = 0; j < X; j++){
			if(array[i][j] == 0){
				if(max == 0){
					max = probability[i][j];
					y = i;
					x = j;
				}
				else{
					if(max >= probability[i][j]){
						int for_rand = All_count(0 , array);
						if(for_rand < 15){
							for_rand += 10;
						}
						else{
							for_rand /= 4;
						}
						if(max > probability[i][j] || (!(rand()%for_rand))){
							max = probability[i][j];
							y = i;
							x = j;
						}
					}
					else{
					}
				}
			}
		}
	}
	printf("RAND !! will click y %d x %d \n" , y , x);
    Sleep(3500);
    open_window("minesweeper");
	Click(x , y , "left");


	return ;
}


void analyze_screen(void){

	int i , j;

	hdc = GetDC(wndPane);

	for(i = 0 ; i < Y ; i++){//
		for(j = 0 ; j < X ; j++){
            analyze_single(i , j);
		}
	}

	return;
}


int analyze_single(int i , int j){

    int r , g, b;
	int k , result = 0;

	for(k = 10 ; k < 29 ; k++){
	    if(array[i][j] == 0 || array[i][j] == 7 || array[i][j] == 8){
        //   ^ ^ v v change position !! would be wrong

    		COLORREF color = GetPixel(hdc, POX + j * BoxLength + k , POY + i * BoxLength + 13);//origin i+6 // later + 13
    		r = GetRValue(color);
            g = GetGValue(color);
            b = GetBValue(color);

//if(1){
		    if(k == 28 && r > 150 && g > r && (g - r <= 20) && array[i][j] != 3
													&& array[i][j] != 7 && array[i][j] != 8 ){ // ?? is it correct ?? k == 26
		        array[i][j] = 10;
		        result = 10;
			    button[i][j] = 1;
			    break;
		    }
		    if(r >= 185 && g <= 60 && b <= 60){ // g b < 15 //r200
			    array[i][j] = 9;
			    result = 9;
			    break;
		    }
		    if(r >= b || g >= b){
			    if(r > g && r >=b ){
			        if(r > 135){ // 140
					    if(r >= 187 && g < 30 && b < 30){ // g b > 200 r 200  (185 60 60
			     	 	    array[i][j] = 9;
			     	 	    result = 9;
					    }
					    else{
						    COLORREF color_check = GetPixel(hdc, POX + j * BoxLength + k + 3 , POY + i * BoxLength + 13);// k+4 ~~ to k+3
						    int r_check = GetRValue(color_check);
						    int g_check = GetGValue(color_check);
						    int b_check = GetBValue(color_check);
						    if(r_check  >= 187 && g_check < 30 && b_check < 30){//190 && r_check > g_check && r_check > b_check){//test >=200 r 200
							    array[i][j] = 9;
							    result = 9;
		      			    }
						    else if(r_check < 140){
							    array[i][j]	= 5;
                                result = 5;
						    }
						    else if( g_check < 60 && b_check < 60){
							    int red_arround_nums_num = neighbor_num(i , j) - Arround(i , j , "flag" , array) - Arround(i , j , "space" , array);
								if(red_arround_nums_num > 1 && Arround(i , j , "flag" , array) <= 3){//<= 3
								    array[i][j] = 3;
								    result = 3;
								}
							    else if(red_arround_nums_num == 1){
									    printf("check %d %d = 7 because %d %d %d\n", i , j , r_check , g_check , b_check);
									    if(Arround(i , j , "flag" , array) <= 3){
										    array[i][j] = 3;
										    result = 3;
									    }
									    else{
										    array[i][j] = 7;
										    result = 7;
										}
							    }
							    else{
							    	array[i][j]	= 8;
							    	result = 8;
							    }
						    }
	           		    }
				    }
				    else{
					    COLORREF color_check = GetPixel(hdc, POX + j * BoxLength + k + 4 , POY + i * BoxLength + 13 ); //
					    int r_check = GetRValue(color_check);
					    int g_check = GetGValue(color_check);
					    int b_check = GetBValue(color_check);

					    if(r_check >= 187 && g_check < 30 && b_check < 30){//test //200
						    array[i][j] = 9;
						    result = 9;
					    }
					    else{
						    array[i][j] = 5;
						    result = 5;
					    }
					    break;
				    }
			    }

			    if(r < g || b <= g){
				    if(r < 70){
					    if(b > 100){
						    array[i][j] = 6;
						    result = 6;
						}
					    else{
						    array[i][j] = 2;
						    result = 2;
						}
					    break;
				    }
			    }
		    }
		    else{

			    if(r < 20 && g < 20 & b > 120 && b < 145){
				    array[i][j] = 4;
				    result = 4;
                    break;
			    }

			    if(r < 15 && (g > 115 && g < 128) && (b > 120 && b < 135) ){
				    array[i][j] = 6;
				    result = 6;
                    break;
			    }
			    if(r > 50 && r < 80 && g > 75 && g < 95 && b < 205 && b > 180 ){
    			    COLORREF color_check = GetPixel(hdc, POX + j * BoxLength + k + 8 , POY + i * BoxLength + 13 ); //
				    int r_check = GetRValue(color_check);
				    int g_check = GetGValue(color_check);
				    int b_check = GetBValue(color_check);
				    if( r_check > 160  && g_check > 170 && b_check > 195){
    			    	array[i][j] = 1;
					    result = 1;
                        button[i][j] = 0;
	       			    break;
				    }
				    else{
				    }
			    }
		    }
//    }//if 1
	    }
	    else{
	       return result;
	    }
	}

	return result;

}

int All_count(int what , int matrix[][X]){
	int i , j , num = 0;
	for(i = 0; i < Y ; i++)
		for(j = 0; j < X; j++)
			if(what == 'd'){
				if(matrix[i][j] == 0)
					if(neighbor_num(i , j) - Arround(i , j , "space" , array) - Arround(i , j , "flag" , array) != 0)
						num++;
			}
			else if(matrix[i][j] == what){
				num++;
			}
	return num;
}
int Arround(int y , int x , char *mod , int matrix[][X]){
	int destination;
	int num = 0;
	if(!strcmp(mod , "flag")) destination = 9;
	else if(!strcmp(mod , "space")) destination = 0;
	else if(!strcmp(mod , "clear")) destination = 10;
	else{
		puts("Arround err");
		exit(0);
	}

    struct point_ * point;
    int which_pt = 0;
    while(point = rtnpt( y , x , which_pt++)){
        if(matrix[point->y][point->x] == destination){
			num++;
		}
        free(point);
    }

	return num;
}
int check_if_win(void){

	print_array("array");
	if(flags <= 2 && flags > 0){
		puts("sleep 2s");
		Sleep(2000);
	}
	int pass = 0;
	int i , j , k;
	int r , g, b;
	hdc = GetDC(wndPane);
	wndGameLost = FindWindow(NULL , "Game Lost");
	if(wndGameLost != NULL){
		LOSE_NUM++;
		puts("game over");
		Sleep(3000);
		exit(0);
		Click(-100 , -100 , "left");
		Sleep(1500);
		return -1;
	}

	wndGameLost = FindWindow(NULL , "Game Won");
	if(wndGameLost != NULL){
		puts("game won");
		WIN_NUM++;
		Sleep(3000);
		Click(100 , 100 , "left");
		Sleep(1500);
		return 1;
	}
	return 0;
}

void print_array(char matrix_name[]){
	int i , j;
    int tem_array[Y][X];
	if(!strcmp(matrix_name,"dfs_count")){
		for(i = 0;i<Y ; i++){
			for(j = 0 ; j < X ; j++){
				if(dfs_count[i][j].num == -1)
					printf("      ");
				else if(neighbor_num(i , j) - Arround(i , j , "space" , array) - Arround(i , j , "flag" , array) == 0)//matrix[i][j] == -2)
					printf("%1.0f?%2d " , dfs_count[i][j].num , dfs_count[i][j].exp);
				else
					printf("%1.0fe%2d " , dfs_count[i][j].num , dfs_count[i][j].exp);
			}
			puts("");
		}
	}
	else{
		if(!strcmp(matrix_name , "array")){
            for(i = 0 ; i != Y ; i ++)
                for(j = 0 ;  j != X ; j ++)
                    tem_array[i][j] = array[i][j];
        }
		else if(!strcmp(matrix_name , "reason_array")){// matrix_one_D = reason_array;
            for(i = 0 ; i != Y ; i ++)
                for(j = 0 ;  j != X ; j ++)
                    tem_array[i][j] = reason_array[i][j];
        }
		else if(!strcmp(matrix_name , "border")){// matrix_one_D = reason_array;
            for(i = 0 ; i != Y ; i ++)
                for(j = 0 ;  j != X ; j ++)
                    tem_array[i][j] = border[i][j];
        }
		else printf("no such array named %s\n" , matrix_name) , exit(0);

		for(i = 0;i<Y ; i++){
			for(j = 0 ; j < X ; j++){
				if(tem_array[i][j] == 9)
					printf("F ");
				else if(tem_array[i][j] == 10 || tem_array[i][j] == -1)
					printf("  ");
				else
					printf("%d " , tem_array[i][j]);
			}
			puts("");
		}
	}

	puts("\n");
	return ;
}

void MouseSetting(void){

	pt.x = MOX;
	pt.y = MOY;

	mDown.mouseData=0;
    mDown.time=0;
    mDown.dwExtraInfo=0;

    mDown.dy=pt.y;
    mDown.dx=pt.x;
    mDown.dwFlags=MOUSEEVENTF_LEFTDOWN;

    inputMouseDown_L.type=INPUT_MOUSE;
    inputMouseDown_L.mi=mDown;

	mUp.mouseData=0;
    mUp.time=0;
    mUp.dwExtraInfo=0;

    mUp.dy=pt.y;
    mUp.dx=pt.x;
    mUp.dwFlags=MOUSEEVENTF_LEFTUP;

    inputMouseUp_L.type=INPUT_MOUSE;
    inputMouseUp_L.mi=mUp;

	nDown.mouseData=0;
    nDown.time=0;
    nDown.dwExtraInfo=0;

    nDown.dy=pt.y;
    nDown.dx=pt.x;
    nDown.dwFlags=MOUSEEVENTF_RIGHTDOWN;

    inputMouseDown_R.type=INPUT_MOUSE;
    inputMouseDown_R.mi=nDown;

	nUp.mouseData=0;
    nUp.time=0;
    nUp.dwExtraInfo=0;

    nUp.dy=pt.y;
    nUp.dx=pt.x;
    nUp.dwFlags=MOUSEEVENTF_RIGHTUP; 

    inputMouseUp_R.type=INPUT_MOUSE; 
    inputMouseUp_R.mi=nUp;

	return ;
}
void Click(int x , int y  , char *mod){
	if((!strcmp(mod , "right")) || (!strcmp(mod , "both"))) button[y][x] = 1;

	if((x == 100 && y == 100)||(x == -100 || y == -100)||(x == 999 && y == 999)){

	}
	else if(x > X || x < 0 || y > Y || y < 0){
		puts("err");
		printf("x %d y %d flags = %d\n" , x, y , flags);
		Sleep(15000);
	}

	UINT num ;

	if((x == 100 && y == 100)){
		pt.x = 763;
		pt.y = 467;
	}
	else if((x == -100 && y == -100)){
		pt.x = 806;//679
		pt.y = 447;//445
	}
	else if((x == 999 && y == 999)){
		pt.x = 1240;
		pt.y = 629;
	}
	else{
		pt.x = MOX + x * BoxLength;
		pt.y = MOY + y * BoxLength;
	}

	SetCursorPos(pt.x , pt.y);

	mDown.dy=pt.y;
    mDown.dx=pt.x;
	mUp.dy=pt.y;
    mUp.dx=pt.x;
    nDown.dy=pt.y;
    nDown.dx=pt.x;
	nUp.dy=pt.y;
    nUp.dx=pt.x;

	INPUT event[4]={inputMouseDown_L , inputMouseUp_L ,
		inputMouseDown_R , inputMouseUp_R};

	if((!strcmp(mod , "left")) || (!strcmp(mod , "both"))){
   		num=SendInput(1,&event[0],sizeof(INPUT));
   		if(num==0){
   			puts("error");
  		}
	}
	if((!strcmp(mod , "right")) || (!strcmp(mod , "both"))){
   		num=SendInput(1,&event[2],sizeof(INPUT));
   		if(num==0){
    		puts("error");
    	}
    }
	if((!strcmp(mod , "left")) || (!strcmp(mod , "both"))){
		num=SendInput(1,&event[1],sizeof(INPUT));
	    if(num==0){
	    	puts("error");
	    }
	}
	if((!strcmp(mod , "right")) || (!strcmp(mod , "both"))){
		num=SendInput(1,&event[3],sizeof(INPUT));
	    if(num==0){
	    	puts("error");
	    }

       if(!strcmp(mod , "right")){
		  flags--;
		  array[y][x] = 9;
		  printf("flag %d %d\n" , y , x);

	   }
        Sleep(90);
	}

	Sleep(10);

 if(add_speed){
  	if(y >= 0 && y < Y && x >= 0 && x < X){
    	int which_pt = 0;
        struct point_ * point;
 		int detect[Y][X];
		memset(detect , 0 , sizeof(detect));
        if(!strcmp(mod , "both")){
            while(point = rtnpt( y , x , which_pt++)){
				detect[point->y][point->x] = 1;
				int result = analyze_single(point->y , point->x);
				if(result >= 1 && result < 7)
					direct_single(point->y , point->x);
				else if(result == 10){
					dp(point->y , point->x , detect);
				}
                free(point);
            }
        }
        else if(!strcmp(mod , "left")){
        	int result = analyze_single(y , x);
        	if(result >= 1 && result < 7){
            	detect[y][x] = 1;
				if(!direct_single(y , x)){
					while(point = rtnpt( y , x , which_pt++)){
						detect[point->y][point->x] = 1;
						result = analyze_single(point->y , point->x);
						if(result >= 1 && result < 7)
							direct_single(point->y , point->x);
						else if(result == 10){
							dp(point->y , point->x , detect);
						}
						free(point);
					}
				}
			}
			else if(result == 10){
				dp(y , x , detect);
			}
        }
        else if(!strcmp(mod , "right")){

            while(point = rtnpt( y , x , which_pt++)){
                direct_single(point->y , point->x);
                free(point);
            }

        }
	}
}
    return ;
}
void dp(int y , int x , int detect[][X]){

	int which_pt = 0;
	struct point_ * point;
	while(point = rtnpt( y , x , which_pt++)){
		if(detect[point->y][point->x])
			continue;

		detect[point->y][point->x] = 1;
		if(array[point->y][point->x] == 0 || array[point->y][point->x] == 7 || array[point->y][point->x] == 8) analyze_single(point->y , point->x);

		if(array[point->y][point->x] >= 1 && array[point->y][point->x] < 7)
			direct_single(point->y , point->x);
		else if(array[point->y][point->x] == 10){
			dp(point->y , point->x , detect);
		}
		free(point);
	}

	return;
}
