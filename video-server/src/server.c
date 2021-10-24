#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

static int listenfd = -1;
static int connfd   = -1;

void my_system(const char *cmd, char *result_msg, int msg_length)
{
    char result[10240] = {0}, buf[1024] = {0}, *p = NULL;
    FILE *fp = NULL;

    if( (fp = popen(cmd, "r")) == NULL )
    {
        printf("popen error!\n");
        return;
    }

    while (fgets(buf, sizeof(buf), fp))
    {
        strcat(result, buf);
    }
    pclose(fp);
    if(p = strchr(result, '\n'))
    {
        *p = '\0';
    }
    snprintf(result_msg, msg_length, "%s", result);
    return;
}

void get_local_ip_addr(char *local_ipaddr, int length)
{
    if(NULL == local_ipaddr)
    {
        fprintf(stderr, "[%s][%d]input is NULL\n", __FILE__, __LINE__);
        return;
    }
    char *cmd = "ifconfig | grep inet | grep -v inet6 | grep -v 127 | sed 's/^[ \t]*//g' | cut -d ' ' -f2";
    my_system(cmd, local_ipaddr, length);
    return;
}

int open_listenfd(char *port)
{
    int optval = 1;
    struct sockaddr_in serveraddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return -1;
    }

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int)) < 0)
    {
        return -1;
    }
    bzero((char *)&serveraddr, sizeof(serveraddr));
    
    serveraddr.sin_family = AF_INET;
    //serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    char local_ip[64] = {0};
    get_local_ip_addr(local_ip, sizeof(local_ip));
    serveraddr.sin_addr.s_addr = inet_addr(local_ip);
    serveraddr.sin_port = htons((unsigned short)atoi(port));

    if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        return -1;

    if (listen(listenfd, 1024) < 0)
        return -1;

    return listenfd;
}

int Open_listenfd(char *port)
{
    int rc;
    if ((rc = open_listenfd(port)) < 0)
        printf("open_listenfd error");
    return rc;
}