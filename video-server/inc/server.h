#ifndef __SERVER_H__
#define __SERVER_H__

int Open_listenfd(int port);
int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int send_nbite(int fd, char *frame_buf, int frame_length);
void get_local_ip_addr(char *local_ipaddr, int length);

#endif