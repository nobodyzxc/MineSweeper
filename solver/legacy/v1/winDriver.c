
void thread_open_game(void *p){
    system("C:\\\"Program Files\"\\\"Microsoft Games\"\\Minesweeper\\minesweeper.exe");
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

    wndPane = GetWindow(wndPaneParent , 5);

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

        Gaming();

        print_array("array");

        puts("over");
    }

    printf("TOTAL GAME NUMBER = %d\nWIN_NUM = %d\nLOSE_NUM = %d \n" , times , WIN_NUM , LOSE_NUM);

    return 0;
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
    int r , g, b , k , result = 0;
    for(k = 10 ; k < 29 ; k++){
        if(array[i][j] == 0 || array[i][j] == 7 || array[i][j] == 8){
            COLORREF color = GetPixel(hdc, POX + j * BoxLength + k , POY + i * BoxLength + 13);
            r = GetRValue(color); g = GetGValue(color); b = GetBValue(color);
            if(k == 28 && r > 150 && g > r && (g - r <= 20) && array[i][j] != 3 && array[i][j] != 7 && array[i][j] != 8){
                array[i][j] = result = 10;
                button[i][j] = 1;
                break;
            }
            if(r >= 185 && g <= 60 && b <= 60){ // g b < 15 //r200
                array[i][j] = result = 9;
                break;
            }
            if(r >= b || g >= b){
                if(r > g && r >=b ){
                    if(r > 135){ // 140
                        if(r >= 187 && g < 30 && b < 30){ // g b > 200 r 200  (185 60 60
                            array[i][j] = result = 9;
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
                                array[i][j]	= 5 = result = 5;
                            }
                            else if( g_check < 60 && b_check < 60){
                                int red_arround_nums_num = neighbor_num(i , j) - Arround(i , j , "flag" , array) - Arround(i , j , "space" , array);
                                if(red_arround_nums_num > 1 && Arround(i , j , "flag" , array) <= 3){//<= 3
                                    array[i][j] = 3;
                                    result = 3;
                                }
                                else if(red_arround_nums_num == 1){
                                    if(Arround(i , j , "flag" , array) <= 3){
                                        array[i][j] = result = 3;
                                    }
                                    else{
                                        array[i][j] = result = 7;
                                    }
                                }
                                else{
                                    array[i][j]	= result = 8;
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
                            //						printf("%2d %2d = F4  %d %d %d // %d %d %d \n",i,j,r,g,b,r_check , g_check , b_check);
                            array[i][j] = 9;
                            result = 9;
                        }
                        else{
                            //									printf("%2d %2d = 5   %d %d %d \n",i,j,r,g,b);
                            array[i][j] = 5;
                            result = 5;
                        }
                        break;
                    }
                }
                if(r < g || b <= g){
                    if(r < 70){
                        //					printf("%2d %2d = 2 || 6   %d %d %d \n",i,j,r,g,b);
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
                    array[i][j] = result = 4;
                    break;
                }

                if(r < 15 && (g > 115 && g < 128) && (b > 120 && b < 135) ){
                    array[i][j] = result = 6;
                    break;
                }
                if(r > 50 && r < 80 && g > 75 && g < 95 && b < 205 && b > 180 ){
                    COLORREF color_check = GetPixel(hdc, POX + j * BoxLength + k + 8 , POY + i * BoxLength + 13 ); //
                    int r_check = GetRValue(color_check);
                    int g_check = GetGValue(color_check);
                    int b_check = GetBValue(color_check);
                    if( r_check > 160  && g_check > 170 && b_check > 195){
                        array[i][j] = result = 1;
                        button[i][j] = 0;
                        break;
                    }
                }
            }
        }
        else{
            return result;
        }
    }
    return result;
}

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
