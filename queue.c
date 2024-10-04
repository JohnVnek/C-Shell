#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include "queue.h"

typedef struct Node
{
    struct Node *next;
    const char *line;
}
Node;

typedef struct Queue
{
    atomic_int *flag;
    Node *head;
    Node *tail;
    int size;
}
Queue;


Queue *intialize_queue()
{
    Queue *q = malloc(sizeof(Queue));
    q->flag = malloc(sizeof(atomic_int));
    atomic_init(q->flag, 0);
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

void grab_lock(Queue *q)
{
    int expected = 0;
    while (!atomic_compare_exchange_weak(q->flag, &expected, 1))
    {
        expected = 0;
    }
}

void release_lock(Queue *q)
{
    atomic_store(q->flag, 0);
}

void push(Queue *q, const char *line)
{
    grab_lock(q);

    Node *curr = malloc(sizeof(Node));
    curr->line = line;
    curr->next = NULL;

    if (q->size == 0)
    {
        q->head = curr;
        q->tail = q->head;
    }
    else
    {
        q->tail->next = curr;
        q->tail = curr;
    }
    q->size++;
    // print_q(q);

    release_lock(q);
}

const char *poll(Queue *q)
{
    const char *res = NULL;
    if (q->size > 0)
    {
        grab_lock(q);
        res = q->head->line;
        // printf("res: %s", res);
        q->head = q->head->next;
        q->size--;
        release_lock(q);
    }
    return res;
}

int size(Queue *q)
{
    return q->size;
}

void print_q(Queue *q)
{
    grab_lock(q);
    printf("Queue:\n");
    if (q->size > 0)
    {
        Node *curr = q->head;
        for (int i = 0; i < q->size - 1; i++)
        {
            printf("Line: %s\n", curr->line);
            curr = curr->next;
        }
        printf("Line: %s\n", curr->line);
    }
    release_lock(q);
}