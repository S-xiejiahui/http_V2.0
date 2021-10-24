#ifndef __QUEUE__H__
#define __QUEUE__H__

typedef struct quque_node
{
    char data[51200];
    int  data_len;
    struct quque_node *next;
}Queue_Node;

typedef struct queue_head
{
    struct quque_node *first;
    struct quque_node *last;
    int queue_length;
}Queue_Head;

Queue_Head *Queue_Init();
int Queue_push(Queue_Head *head, char *data, int length);
int Queue_pop(Queue_Head *head, char *data, int *length);
int Queue_Emply(Queue_Head *head);
int Queue_Destruction(Queue_Head *head);

#endif