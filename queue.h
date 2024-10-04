#ifndef Queue_H
#define Queue_H

#include <stdatomic.h>


typedef struct Node Node;
typedef struct Queue Queue;

Queue *intialize_queue();
void grab_lock(Queue *q);
void release_lock(Queue *q);
void push(Queue *q, const char *line);
const char *poll(Queue *q);
int size(Queue *q);
void print_q(Queue *q);

#endif