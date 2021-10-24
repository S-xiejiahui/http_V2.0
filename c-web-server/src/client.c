#include "public.h"
#include "server.h"
#include "queue.h"

#include <pthread.h>

static Queue_Head *head = NULL;
static int receive_video_flag = 1;

int stop_receive_video_data(void)
{
    receive_video_flag = 0;
}

int get_a_frame_of_image(char *data, int *length)
{
    if(Queue_pop(head, data, length))
    {
        return 1;
    }
    return 0;
}

int start_receive_video_data(int port)
{
    head = Queue_Init();
    int socket_fd = connect_server(port);
    if(-1 == socket_fd)
    {
        printf("[%s][%d]connect failed\n", __FILE__, __LINE__);
        return -1;
    }
    char data[51200] = {0};
    while (receive_video_flag)
    {
        memset(data, 0x00, sizeof(data));
        int read_n = rio_readn(socket_fd, data, sizeof(data));
        if(read_n < 0)
        {
            printf("[%s][%d]read server data failed\n", __FILE__, __LINE__);
            return -1;
        }
        if(read_n > 0 && head->queue_length < 100)
        {
            Queue_push(head, data, read_n);
        }
    }
    printf("[%s][%d]end of recive...\n", __FILE__, __LINE__);
    return 0;
}

