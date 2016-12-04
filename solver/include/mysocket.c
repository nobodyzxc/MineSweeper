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

#include"mysocket.h"

/* online count */
int G_OL_COUNT;

struct _client *G_CLIENT;

int new_host(int clt_num , int port, void (*recvmsg)(void *idx)){
	int i , idx , sockfd;
    /* leave one to refuse */
    G_CLIENT = (struct _client *)malloc(sizeof(struct _client) * (clt_num + 1));

	for( i = 0 ; i < clt_num + 1 ; i++)
		G_CLIENT[i].fd = -1;
	
    struct sockaddr_in svr_addr;
	G_OL_COUNT = 0;

	/* create socket , same as client if i didn't use () outer socket() */
	/* < 0 would be wrong */
	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		puts("socket() <0");
		exit(1);
	}

	/* initialize structure svr_addr */
	bzero(&svr_addr, sizeof(svr_addr));
	svr_addr.sin_family = PF_INET;
	svr_addr.sin_port = htons(port);

	/* this line is different from client */
	svr_addr.sin_addr.s_addr = INADDR_ANY;

	/* Sign a port number to socket */
	if(bind(sockfd, (struct sockaddr*)&svr_addr, sizeof(svr_addr))==-1){
		puts("bind() rtn -1");
		exit(1);
	}

	/* make it listen to socket with max N + 1 connections */
	listen(sockfd , clt_num + 1);

	puts("wait client connect");

	while(1){

		for(idx = 0 ; idx < clt_num + 1 ; idx++)
			if(G_CLIENT[idx].fd == -1)  break;
		
		struct sockaddr_in client_addr;
		int addrlen = sizeof(client_addr);

		/* Wait and Accept connection */
		G_CLIENT[idx].fd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
		if(G_OL_COUNT == clt_num){
			close(G_CLIENT[idx].fd);
			G_CLIENT[idx].fd = -1;
		}
		else{ /* so i use else here */
			if(pthread_create(&G_CLIENT[idx].td , NULL , (void *)recvmsg , &idx)){
				printf("create client[%d] thread failed\n",idx);
				close(G_CLIENT[idx].fd);
				G_CLIENT[idx].fd = -1;
			}
		}
		sleep(2);
		/* be careful , 
         * because loop is so fast that idx++ before give recvmsg */
	}
	return 0;
}

int cnt_host(int port , char IP[] , void (*recvmsg)(void *svr_fd)){

    int serverfd;
	struct sockaddr_in svr_addr;
	puts(IP);
	printf("will connect to %s\n",IP);
	/* create socket */
	serverfd = socket(PF_INET, SOCK_STREAM, 0);

	/* initialize value in svr_addr */
	bzero(&svr_addr, sizeof(svr_addr));
	svr_addr.sin_family = PF_INET;
	svr_addr.sin_port = htons(port);
	inet_aton(IP, &svr_addr.sin_addr);
	/* Connecting to server */
	if(connect(serverfd, (struct sockaddr*)&svr_addr, sizeof(svr_addr)) == -1){
		puts("connect() rtn -1");
		return -1;
	}
	else{
		puts("please wait");
	}
	/* listen msg */
	pthread_t id;
	if(pthread_create(&id , NULL , (void *)client_recvmsg , &serverfd)){
		printf("create pthread error!\n");
		exit(1);
	}
    /* wait a sec to pass serverfd into recvmsg */ 
	return serverfd;
}

void server_recvmsg(void *clt_idx){
	char buffer[310];/*to receive msg */
	int idx = *((int*)clt_idx);
	int len;

	G_OL_COUNT++;

	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL);

	while(1){
		len = recv(G_CLIENT[idx].fd , buffer , sizeof(buffer) , 0);
		if(len == 0){
			printf("G_CLIENT[%d] connection break\n" , idx);
			break;
		}
		else if(len >= 1){
			puts(buffer);
		}
		else{
			/*connect error */
			printf("receive error msg[%d] from %s\n",len,G_CLIENT[idx].name);
		}
	}

	G_OL_COUNT--;/*if client break by himself */
	/* Close connection */
	close(G_CLIENT[idx].fd);
	G_CLIENT[idx].fd = -1 ;
	G_CLIENT[idx].name[0] = '\0';
	pthread_exit(0);
	return;
}

void client_recvmsg(void *svr_fd){
	char buffer[310];
	int len;
    int serverfd = *((int*)svr_fd);
	while(1){
	/* Receive message from the server and print to screen */
		len = recv(serverfd, buffer, sizeof(buffer),0);

		if(len == 0){
			puts("connection break");
			break;
		}
		else if(len >= 1){
			puts(buffer);
		}
		else{
			puts("receive error msg");
		}
	}
	close(serverfd);
	return;
}

int combsys(char *cmd , unsigned int cmd_t , char *format , ... ){
    va_list arg;
    va_start(arg , format);
    vsnprintf(cmd , cmd_t , format , arg);
    va_end(arg);
    return system(cmd);
}

ssize_t combsend(int fd , char *msg , unsigned int msg_t , char *format , ... ){
    va_list arg;
    va_start(arg , format);
    vsnprintf(msg , msg_t , format , arg);
    va_end(arg);
    return send(fd , msg , msg_t , 0);
}
