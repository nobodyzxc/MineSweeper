#ifndef _MYSOCKET_H_
#define _MYSOCKET__H_

#define LOCAL     "127.0.0.1"
struct _client{
    pthread_t td;
    int root;
    int fd;
    char name[20];
    char curname[20];
};

int new_host(int clt_num , int PORT, void (*recvmsg)(void *clt_idx));
int cnt_host(int PORT , char IP[] , void (*recvmsg)(void *svr_fd));

/* referencing for func pointer above */
   void server_recvmsg(void *clt_idx);
   void client_recvmsg(void *svr_fd);
#endif
