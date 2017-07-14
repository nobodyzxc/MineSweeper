#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#include<string.h>
#include<strings.h>
#include<math.h>

#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<pthread.h>
#include<signal.h>
#include<unistd.h>

#include"include/mysocket.h"
#include"GameSol.c"

extern int **G_MAP;

void recvSvr(void *svr_fd);

int main(void){
    char sendStr[100];

    if(!(G_MAP = (int**) malloc
                (G_MAPLN * sizeof(void*) + G_MAPLN *G_MAPWD * sizeof(int))))
		exit(1);

	int *tmp = (int *)(G_MAP + G_MAPLN);
	for(i = 0; i != G_MAPLN ; i++ , tmp += G_MAPWD)
		G_MAP[i] = tmp;

	for(i = 0; i < G_MAPLN ; i++)
		for(j = 0; j < G_MAPWD ; j++)
			G_MAP[i][j] = 0;

    int svrfd = cnt_host(8888 , LOCAL , recvSvr);
    while(1);
    return 0;
}

void recvSvr(void *svr_fd){
	char buffer[256];
	int len , i , j;
    int serverfd = *((int*)svr_fd);

    combsend(svrfd , sendStr , sizeof(sendStr) , 
            "map %d %d %d" , MAPLN , MAPWD , BOMBNUM);

	while(1){
        sol();
	    // Receive message from the server and print to screen
		len = recv(serverfd, buffer, sizeof(buffer),0);
		if(len == 0){
			puts("connection break");
			break;
		}
		else if(len >= 1){
            int ptr = 0 , n = 0;
            char str[10];
		    for(i = 0 ; i < G_MAPLN ; i++){
                for(j = 0 ; j < G_MAPWD ; j++){
//                    sscanf(buffer + ptr , "%s%n" , str , &n);
//                    ptr += n;
                    int pos = i * G_MAPWD + j;
                    if(buffer[pos] == 'F') G_MAP[i][j] = 9;
                    else if(buffer[pos] == '?') G_MAP[i][j] = 10;
                    else if(buffer[pos] >= '0' && 
                            buffer[pos] <= '9')
                        G_MAP[i][j] = buffer[pos] - '0';
                    else if(buffer[pos] == 0) break;
                    else
                        printf("%c" , buffer[pos]);
                }
                if(buffer[pos] == 0) break;
            }
		}
		else{
			puts("receive error msg");
		}
	}
	close(serverfd);
    exit(0);
	return;
}
