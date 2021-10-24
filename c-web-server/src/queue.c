#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

Queue_Head *Queue_Init()
{
    Queue_Head *head = NULL;
    head = (Queue_Head*)malloc(sizeof(Queue_Head));
    head->first = NULL;
    head->last = NULL;
    head->queue_length = 0;
    return head;
}

int Queue_push(Queue_Head *head, char *data, int length)
{
    if (NULL == head)
    {
        head = (Queue_Head*)malloc(sizeof(Queue_Head));
        head->first = NULL;
        head->last = NULL;
        head->queue_length = 0;
    }

    Queue_Node *p = (Queue_Node*)malloc(sizeof(Queue_Node));
    p->next = NULL;
    p->data_len = length;
    strncpy(p->data, data, length);
    if (NULL == head->first)
    {
        head->first = head->last = p;
    }
    else
    {
        head->last->next = p;
        head->last = p;
    }
    head->queue_length++;
    return 1;
}

int Queue_Emply(Queue_Head *head)
{
    if (NULL == head)
    {
        return -1;
    }
    return head->queue_length;
}

int Queue_pop(Queue_Head *head, char *data, int *length)
{
    if (NULL == head)
    {
        printf("[%s][%d]: input is NULL\n", __FILE__, __LINE__);
        return 0;
    }
    if(Queue_Emply(head))
    {
        *length = head->first->data_len;
        strncpy(data, head->first->data, head->first->data_len);
        Queue_Node *temp = head->first;
        head->first = head->first->next;
        head->queue_length--;
        free(temp);
        return 1;
    }
    return 0;
}

int Queue_Destruction(Queue_Head *head)
{
    if(NULL == head)
    {
        return -1;   
    }
    while (Queue_Emply(head))
    {
        Queue_Node *temp = head->first;
        head->first = head->first->next;
        head->queue_length--;
        free(temp);
    }
    return 0;
}
#if 0
int main(int argc, char const *argv[])
{
    Queue_Head *head = Queue_Init();
    Queue_push(head, "123", 7);
    Queue_push(head, "aaa", 5);
    Queue_push(head, "456", 7);
    Queue_push(head, "bbb", 5);
    Queue_push(head, "789", 7);
    Queue_push(head, "ccc", 5);

    char buf[32] = {0};
    while (Queue_Emply(head))
    {
        Queue_pop(head, buf, 32);
        printf("buf = %s\n", buf);
    }
    
    return 0;
}
#endif