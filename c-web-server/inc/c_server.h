#ifndef __SERVER_H__
#define __SERVER_H__

/* create Server */
int Open_listenfd(int port, const char *ip);

/* create client */
int connect_server(int port);

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
void get_local_ip_addr(char *local_ipaddr, int length);
void my_system(const char *cmd, char *result_msg, int msg_length);
#endif