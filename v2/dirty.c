int need_flag(void){
    int i , j;
    for(i = 0 ; i < MAPHI ; i++)
        for(j = 0 ; j < MAPWD ; j++){
            POINT pt = {j , i};
            if(HASBOMB(rsnMap , pt)){
                if(PTON(pt , rsnMap) > adjCnt(pt , FLG , rsnMap)
                        && adjCnt(pt , UNK , rsnMap) > 0)
                    return 1;
            }
        }
    return 0;
}

bool legalRsn(POINT argpt){

    int rtn = true , idx = 0;
    POINT **adjptr = adjPts(argpt);
    for(idx = 0 ; idx < 8 && adjptr[idx] ; idx++){
        if(PTON(*adjptr[idx] , rsnMap) > 0
                && PTON(*adjptr[idx] , rsnMap) < 9){
            int flgs = adjCnt(*adjptr[idx] , FLG , rsnMap);
            int unks = adjCnt(*adjptr[idx] , UNK , rsnMap);
            if(PTON(*adjptr[idx] , rsnMap) < flgs)
                rtn = false; /* too many flags */
            else if(PTON(*adjptr[idx] , rsnMap) > flgs
                    && unks == 0)
                rtn = false; /* no space left to add flags */
        }
    }

    for(idx = 0 ; idx < 8 ; idx++)
        free(adjptr[idx]);
    free(adjptr);

    if(need_flag()){ /* if whole map still needs flag */
        if(dfs_flags <= 0){
            puts("need flags while no space left");
            rtn = false;
        }
    }
    else{
        puts("no need flags , maybe have result");
        getchar();
        printTab(rsnMap);
    }
    return rtn;
}

void print_map(const char *matrix_name) {
    int i, j;
    int tem_map[MAPHI][MAPWD];
    if (!strcmp(matrix_name, "dfs_count")) {
        for (i = 0; i < MAPHI; i++) {
            for (j = 0; j < MAPWD; j++){
                POINT pt = {j , i};
                if (dfs_count[i][j].num == -1)
                    printf("      ");
                else if (adjPtNum(pt) - adjCnt(pt , UNK , map) - adjCnt(pt , FLG , map) == 0) //matrix[i][j] == -2)
                    printf("%1.0f?%2d ", dfs_count[i][j].num, dfs_count[i][j].exp);
                else
                    printf("%1.0fe%2d ", dfs_count[i][j].num, dfs_count[i][j].exp);
            }
            puts("");
        }
    } else {
        if (!strcmp(matrix_name, "map")) {
            // matrix_one_D = map;
            for (i = 0; i != MAPHI; i++)
                for (j = 0; j != MAPWD; j++)
                    tem_map[i][j] = map[i][j];
        } else if (!strcmp(matrix_name, "rsnMap")) { // matrix_one_D = rsnMap;
            for (i = 0; i != MAPHI; i++)
                for (j = 0; j != MAPWD; j++)
                    tem_map[i][j] = rsnMap[i][j];
        } else if (!strcmp(matrix_name, "border")) { // matrix_one_D = rsnMap;
            for (i = 0; i != MAPHI; i++)
                for (j = 0; j != MAPWD; j++)
                    tem_map[i][j] = border[i][j];
        } else printf("no such map named %s\n", matrix_name), exit(0);

        for (i = 0; i < MAPHI; i++) {
            for (j = 0; j < MAPWD; j++) {
                //				matrix_two_D = (matrix_one_D[i]);
                if (tem_map[i][j] == 9)
                    printf("F ");
                else if (tem_map[i][j] == 10 || tem_map[i][j] == -1)
                    printf("  ");
                else
                    printf("%d ", tem_map[i][j]);
            }
            puts("");
        }
    }
    puts("\n");
    return;
}

int resuppose(void){

    int i , j , k , l;
    int tem_num;
    int exist_unknown = 0;
    int result = 0;

    analyMap(true);

    game_plat_possible = 0;
    left_mine_unknown = 0;

    print_map("map");
    flags = (float)(FLAGNUM - cntMap(FLG , map));
    printf("flags == %f ,FLAGNUM is %d ,  cntMap 9 is %d \n" ,
            flags , FLAGNUM , cntMap(9 , map));
    dfs_flags = flags;
    printf("dfs_flags = %f\n" , dfs_flags);

    //initial dfs_count

    for(k = 0 ; k < MAPHI ; k++)
        for(l = 0 ; l < MAPWD ; l++ ){
            dfs_count[k][l].num = 0;
            dfs_count[k][l].exp	= 0;
        }

    //initial rsnMap & dfs_count

    for(k = 0; k < MAPHI ; k++){
        for(l = 0 ; l < MAPWD ; l++){
            POINT pt = {l , k};

            rsnMap[k][l] = map[k][l];

            if(map[k][l] != 0){
                dfs_count[k][l].num = -1;
            }
            else if(adjPtNum(pt) - adjCnt(pt , UNK , map) - adjCnt(pt , FLG , map) == 0 ){
                dfs_count[k][l].num = 0;// == -2
                exist_unknown = 1;
            }
            else{
                dfs_count[k][l].num = 0;
            }
        }
    }

    out_to_rand = 0;
    //something strange, so i make border again

    POINT pt = {j , i};
    if(map[i][j] == 0 && tem_num < adjPtNum(pt) && tem_num > 0)// **
        border[i][j] = 1;
    else
        border[i][j] = 0;

    POINT zero = {0 , 0};
    dfs(zero);
    puts("out dfs");

    if(out_to_rand == 10000){
        gessClk();
        return result;//question?
    }
    for(i = 0 ; i < MAPHI ; i++)
        for(j = 0 ; j < MAPWD ; j++){
            //			printf("%d %d\n" , i , j);
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

    printf("MAPWD = %d MAPHI= %d\n" , MAPWD , MAPHI);
    puts("resuppose return\n\n");

    print_map("dfs_count") ;//// hereherehereherehere
    printf("All game plat sum == %d\n" , game_plat_possible);
    if(game_plat_possible == 0) exit(0);
    printf("left mine unknow %s\n" , left_mine_unknown ? "TRUE" : "FALSE");
    Sleep(2000);
    getchar();

    if(exist_unknown == 1 && left_mine_unknown == 0){
        puts("start click which blocks for sure");
        getchar();
        Sleep(2500);

        //open_window("minesweeper");
        Sleep(1000);
        result = 1;
        for(k = 0; k < MAPHI ; k++){
            for(l = 0 ; l < MAPWD ; l++){
                POINT pt = {l , k};
                if(adjPtNum(pt) - adjCnt(pt , UNK , map) - adjCnt(pt , FLG , map) == 0){
                    click(pt , LEFT);
                }
            }
        }
    }

    int bestX = 0 , bestY = 0 , bestchosen = -3;
    int best_total = 1;

    for(k = 0 ; k < MAPHI ; k++){
        for(l = 0 ; l < MAPWD ; l++){
            POINT pt = {l , k};
            if(dfs_count[k][l].num == 0 && dfs_count[k][l].exp == 0 && adjPtNum(pt) - adjCnt(pt , UNK , map) - adjCnt(pt , FLG , map) != 0){
                if(result == 0){
                    puts("click bolcks for sure");
                    getchar();
                    //open_window("minesweeper");
                    Sleep(1000);
                }
                click(pt , LEFT);
                result = 1;
            }
            if(dfs_count[k][l].num >= 0){
                if(bestchosen == -3){
                    bestY = k;
                    bestX = l;
                    bestchosen = 0;
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


    //open_window("console");
    Sleep(1000);

    if(result == 0){

        if(game_plat_possible == 1) {
            puts("only one plat , will rand!!");
            Sleep(2500);
            gessClk() ;
            return 0;
        }
        else{


            printf("best total is %d\n" , best_total);
            int rand_num = rand() % best_total ;

            printf("chose NO.%d pt(start from 0)\n" , rand_num);

            for(k = 0 ; k < MAPHI ; k++){

                for(l = 0 ; l < MAPWD ; l++){
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

                if(rand_num == 0 && l < MAPWD){ // if i doesn't add l < MAPWD, it will be wrong , it's so important!!
                    break;
                }

            }
            POINT pt = {l , k};
            printf("will click bestY = %d , bestX = %d\n" , k , l);
            printf("num = %1.1f , exp = %d\n" , dfs_count[k][l].num , dfs_count[k][l].exp);
            printf("is ( %d )\n" , rsnMap[k][l]);
            puts("Enter to continue...");
            getchar();
            //open_window("minesweeper");
            click(pt , LEFT);
        }
    }
    else{
        puts("plat have change, will continue game");
        Sleep(2500);
        //open_window("minesweeper");
    }

    game_plat_possible = 0;
    left_mine_unknown = 0;

    return result;
}

struct notation C(int P , int C){
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

void dfs(POINT argpt){

    out_to_rand++;

    if(out_to_rand % 10000 == 0){
        printf("out of %d \n" , out_to_rand);
        if(out_to_rand == 10000) return;
        Sleep(5000);
    }

    int i , j , k , l , tem_num;

    for(i = 0 ; i < MAPHI ; i++){
        for(j = 0 ; j < MAPWD ; j++){
            POINT pt = {j , i};
            if(argpt.y != 0 && argpt.x != 0 && ((i < argpt.y ) || (i == argpt.y && j <= argpt.x))) continue;

            printf("suppose into %d %d\n" , i , j);

            if(PTON(pt , rsnMap) == UNK){

                if(border[i][j]){

                    if(cntMap(UNK , rsnMap) - 1 >= dfs_flags){
                        rsnMap[i][j] = SAF;

                        if(legalRsn(pt)) dfs(pt);
                        /* else if(result == 1)
                            puts("AAAAA???");
                            out_to_rand = 10000;
                            return;
                        } */

                    }

                    puts("into flags");

                    if(dfs_flags >= 1){

                        printf("%d %d supplse as flags\n" , i , j);
                        rsnMap[i][j] = 9;
                        dfs_flags--;

                        if(legalRsn(pt)){
                            dfs(pt);
                            dfs_flags++;
                        }
                        else{
                            if(dfs_flags == 0){//nbodyzxc
                                printf("\n%d %d  -- = falgs = %f \n" , i , j , dfs_flags );
                                puts("\n\n may right vvv\n\n");
                                puts("-----------flags no left !!!!-------------");
                                print_map("rsnMap");
                                for(k = 0 ; k < MAPHI ; k++)
                                    for(l = 0 ; l < MAPWD ; l++){
                                        if(dfs_count[k][l].num != -1)
                                            if(rsnMap[k][l] == 9)
                                                dfs_count[k][l].num++;
                                    }

                                dfs_flags++;
                                game_plat_possible ++;
                            }
                            else{// == -1 || flags != 0

                                int result = 0;
                                //TODO check
                                if(result == 1 || result == 0){

                                    int a , b;
                                    int backup[MAPHI][MAPWD];

                                    //backup becuase the op below
                                    for(a = 0 ; a != MAPHI ; a++)
                                        for( b = 0 ; b != MAPWD ; b++)
                                            backup[a][b] = rsnMap[a][b];

                                    // transfer the 0 on border to 10

                                    for(a = 0 ; a != MAPHI ; a++)
                                        for( b = 0 ; b != MAPWD ; b++){
                                            POINT pt = {b , a};
                                            if(rsnMap[a][b] != 0){
                                                if(adjCnt(pt , FLG , rsnMap) == rsnMap[a][b] && adjCnt(pt , UNK , rsnMap) != 0){
                                                    int idx;
                                                    POINT **adjptr = adjPts(pt);
                                                    for(idx = 0 ; idx < 8 && adjptr[idx] ; idx++){
                                                        if(!PTON(*adjptr[idx] , rsnMap))
                                                            PTON(*adjptr[idx] , rsnMap) = SAF;
                                                    }
                                                    for(idx = 0 ; idx < 8 ; idx++)
                                                        free(adjptr[idx]);
                                                    free(adjptr);
                                                }
                                            }
                                        }


                                    // start calclulate Combanation
                                    struct notation temC;
                                    temC = C(cntMap(UNK , rsnMap) , (int)dfs_flags);
                                    printf("A 0 %d ,flags %d\n" ,cntMap(UNK , rsnMap) , (int)dfs_flags);
                                    printf("add %f %d\n" , temC.num , temC.exp);
                                    //									getchar();
                                    for(k = 0 ; k < MAPHI ; k++)
                                        for(l = 0 ; l < MAPWD ; l++){
                                            POINT pt = {l , k};
                                            if(dfs_count[k][l].num >= 0)
                                                if(rsnMap[k][l] == 9){
                                                    addC( &dfs_count[k][l] , temC);
                                                }
                                                else if(adjPtNum(pt) - adjCnt(pt , UNK , map) - adjCnt(pt , FLG , map) == 0 && rsnMap[k][l] == 0){
                                                    if((k == 14 || k == 15)&&(l >= 24 && l <= 26))printf("=> %d %d \n" , k , l);
                                                    addC( &dfs_count[k][l] , temC);
                                                }
                                        }
                                    printf("----------flags left %f---------\n\n\n" , dfs_flags);
                                    left_mine_unknown = 1; // here !!
                                    print_map("rsnMap");
                                    puts("^  ^^  ^ final !!!\n\n\n");
                                    game_plat_possible ++;
                                    //recover rsnMap
                                    for(a = 0 ; a != MAPHI ; a++)
                                        for( b = 0 ; b != MAPWD ; b++)
                                            rsnMap[a][b] = backup[a][b];

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
                            sleep(20);
                            exit(0);
                        }
                    }

                    rsnMap[i][j] = 0;

                    return;
                }
            }
        }
    }

    int a , b;
    int backup[MAPHI][MAPWD];
    //backup becuase the op below
    for(a = 0 ; a != MAPHI ; a++)
        for( b = 0 ; b != MAPWD ; b++)
            backup[a][b] = rsnMap[a][b];

    // transfer the 0 on border to 10

    for(a = 0 ; a != MAPHI ; a++)
        for( b = 0 ; b != MAPWD ; b++){
            POINT pt = {b , a};
            if(rsnMap[a][b] != 0){
                if(adjCnt(pt , FLG , rsnMap) == rsnMap[a][b] && adjCnt(pt , UNK , rsnMap) != 0){
                    int idx;
                    POINT **adjptr = adjPts(pt);
                    for(idx = 0 ; idx < 8 && adjptr[idx] ; idx++){
                        if(!PTON(*adjptr[idx] , rsnMap))
                            PTON(*adjptr[idx] , rsnMap) = SAF;
                    }
                    for(idx = 0 ; idx < 8 ; idx++)
                        free(adjptr[idx]);
                    free(adjptr);
                }
            }
        }

    // start calclulate Combanation
    struct notation temC;
    temC = C(cntMap(UNK , rsnMap) , (int)dfs_flags);
    printf("A 0 %d ,flags %d\n" ,cntMap(UNK , rsnMap) , (int)dfs_flags);
    printf("add %f %d\n" , temC.num , temC.exp);
    for(k = 0 ; k < MAPHI ; k++)
        for(l = 0 ; l < MAPWD ; l++){
            POINT pt = {l , k};
            if(dfs_count[k][l].num >= 0)
                if(rsnMap[k][l] == 9){
                    addC(&dfs_count[k][l] , temC);
                }
                else if(adjPtNum(pt) - adjCnt(pt , UNK , map) - adjCnt(pt , FLG , map) == 0 && rsnMap[k][l] == 0){
                    if((k == 14 || k == 15)&&(l >= 24 && l <= 26))printf("=> %d %d \n" , k , l);
                    addC(&dfs_count[k][l] , temC);
                }
        }
    printf("----------flags left %f---------\n\n\n" , dfs_flags);
    left_mine_unknown = 1; // here !!
    print_map("rsnMap");
    puts("^      ^ final !!!\n\n\n");
    game_plat_possible ++;

    //recover rsnMap
    for(a = 0 ; a != MAPHI ; a++)
        for( b = 0 ; b != MAPWD ; b++)
            rsnMap[a][b] = backup[a][b];


    return ;
}

void gessClk(void){
    int arround_tem;
    POINT pt;
    int i , j ;
    float num_of_unknown = 0 , arround_to_add;
    float probability[MAPHI][MAPWD] = {0} , max = 0;

    puts("\nstart clkrand !!\n");


    for(i = 0; i < MAPHI ; i++){
        for(j = 0; j < MAPWD; j++){
            if(map[i][j] == 0) num_of_unknown++;
        }
    }


    if(num_of_unknown == 0){
        puts("num_of_unknown == 0");
        exit(0);
    }

    for(i = 0; i < MAPHI ; i++){
        for(j = 0; j < MAPWD; j++){
            POINT pt = {j , i};
            if(map[i][j] != 0 && map[i][j] != 9 &&
                    map[i][j] != 10){

                arround_tem = adjCnt(pt , UNK , map);

                if(arround_tem == 0){
                    continue;
                }
                else{
                    arround_to_add = ((float)(map[i][j] - adjCnt(pt , FLG , map)) / (float)arround_tem);
                }
                int idx;
                POINT **adjptr = adjPts(pt);
                for(i = 0 ; i < 8 && adjptr[idx] ; i++){
                    if(!PTON(*adjptr[idx] , map)){
                        if(arround_tem)
                            PTON(*adjptr[idx] , probability) += (1 - PTON(*adjptr[idx] , probability)) * arround_to_add;
                    }
                }
                for(i = 0 ; i < 8 ; i++)
                    free(adjptr[idx]);
                free(adjptr);
            }
        }
    }

    for(i = 0; i < MAPHI ; i++){
        for(j = 0; j < MAPWD ; j++){
            if(map[i][j] == 0){
                if(num_of_unknown != 0)
                    if(probability[i][j] == 0)
                        probability[i][j] = flags / num_of_unknown;
            }
        }
    }


    for(i = 0; i < MAPHI ; i++){
        for(j = 0; j < MAPWD; j++){
            if(map[i][j] == 0){
                if(max == 0){
                    max = probability[i][j];
                    //					printf("max = %d %d is %f\n" , i , j , max);
                    pt.y = i;
                    pt.x = j;
                }
                else{
                    if(max >= probability[i][j]){
                        int for_rand = cntMap(UNK , map);
                        if(for_rand < 15){
                            //							for_rand = cntMap(0);
                            for_rand += 10;
                        }
                        else{
                            for_rand /= 4;
                        }
                        if(max > probability[i][j] || (!(rand()%for_rand))){
                            max = probability[i][j];
                            pt.y = i;
                            pt.x = j;
                        }
                    }
                }
            }
        }
    }
    printf("RAND !! will click y %d x %d \n" , pt.y , pt.x);
    sleep(3);
    click(pt , LEFT);
    return ;
}
