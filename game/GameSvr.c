#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<math.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/stat.h>

#include<pthread.h>
#include<signal.h>
#include<unistd.h>
#include<stdbool.h>

#include"include/mysocket.h"
#include"GameMain.h"

extern struct _client *G_CLIENT;
extern int G_OL_COUNT;
extern int **G_MAP , G_BOMBCOUNT , G_MAPLN , G_MAPWD;

void peek(int fd);
void recvPlayer(void *clt_idx);

int main(void){
    new_host(2 , 8888, recvPlayer);
    return 0;
}

void recvPlayer(void *clt_idx){
	char buffer[256];//to receive msg
	int idx = *((int*)clt_idx);
	int len;

	G_OL_COUNT++;

	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL);

	while(1){
		len = recv(G_CLIENT[idx].fd , buffer , sizeof(buffer) , 0);
		if(len == 0){
			printf("client[%d] connection break\n" , idx);
			break;
		}
		else if(len >= 1){
            char arg[5][100];
            int i = 0 , ptr = 0 , n = 0;
            while(~sscanf(buffer + ptr , "%s%n" , arg[i] , &n) && i < 5) ptr += n , i++;
            printf("argc = %d\n" , i);
            if(!strcmp("map" , arg[0]) && i == 4) 
                gameStart(atoi(arg[1]) , atoi(arg[2]) , atoi(arg[3]));
            else if(!strcmp("open" , arg[0]) && i == 3) run('s' , atoi(arg[1]) , atoi(arg[2]));
            else if(!strcmp("flag" , arg[0]) && i == 3) run('f' , atoi(arg[1]) , atoi(arg[2]));
            else if(!strcmp("ques" , arg[0]) && i == 3) run('?' , atoi(arg[1]) , atoi(arg[2]));
            else if(!strcmp("look" , arg[0])) peek(G_CLIENT[idx].fd);
            else printf("unknown command %s\n" , buffer);
            printMap(1);
            if(CheckIfWin()){
                free(G_MAP);
                puts("Find All Land Mines !");
                break;
            }
		}
		else{
			//connect error
			printf("receive error msg[%d] from %s\n",len,G_CLIENT[idx].name);
		}
	}

	G_OL_COUNT--;//if client break by himself
	// Close connection 
	close(G_CLIENT[idx].fd);
	G_CLIENT[idx].fd = -1 ;
	G_CLIENT[idx].name[0] = '\0';
	pthread_exit(0);
	return;
}

void peek(int fd){
    int i , j;
    char msgStr[2 * G_MAPLN * G_MAPWD] , apd;
    for(i = 0 ; i < G_MAPLN ; i++){
        for(j = 0 ; j < G_MAPWD ; j++){
             if(hasFLAG(G_MAP[i][j])) apd = 'F';
            else if(hasQUES(G_MAP[i][j])) apd = '?';
            else if(hasNoMark(G_MAP[i][j])) apd = 'O';
			else if(hasOPENED(G_MAP[i][j])) apd = ArroundNum(i , j) + '0';
			sprintf(msgStr , "%s%c ", msgStr , apd);
        }
    }
    send(fd , msgStr , sizeof(msgStr) , 0);
    return;
}
