#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void initialize(ErrHandler *handler)
{
    handler->err_code = 0;
    handler->flag = malloc(sizeof(atomic_int));
    atomic_init(handler->flag, 0);
}

void grab_err_lock(ErrHandler *handler)
{
    int expected = 0;
    while(!atomic_compare_exchange_weak(handler->flag, &expected, 1))
    {
        expected = 0;
    }
}

void release_err_lock(ErrHandler *handler)
{
    atomic_store(handler->flag, 0);
}

void update_err(ErrHandler *handler, int val, char *type)
{
    grab_err_lock(handler);
    handler->err_code = val;
    handler->type = type;
    release_err_lock(handler);
}

int read_err_code(ErrHandler *handler)
{
    grab_err_lock(handler);
    int num = handler->err_code;
    release_err_lock(handler);
    return num;
}

void err_message(ErrHandler *handler)
{
    switch (handler->err_code)
    {
    case 1:
        printf("The command %s requires parameter(s).\n", handler->type);
        break;
    case 2:
        printf("The command %s cannot take parameter(s).\n", handler->type);
        break;
    case 3:
        printf("The command %s was not recognized.\n", handler->type);
        break;
    case 4:
        printf("The command %s cannot have an input.\n", handler->type);
        break;
    case 5:
        printf("The command %s cannot have an output.\n", handler->type);
    case 6:
        printf("The file in the command %s was not found.\n", handler->type);
    case 7:
        printf("The command %s requires input.\n", handler->type);
    default:
        break;
    }
}