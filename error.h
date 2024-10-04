#ifndef ERROR_H
#define ERROR_H

#include <stdatomic.h>

typedef struct ErrHandler
{
    int err_code;
    atomic_int *flag;
    char *type;
}
ErrHandler;

void initialize(ErrHandler *handler);
void grab_err_lock(ErrHandler *handler);
void release_err_lock(ErrHandler *handler);
void update_err(ErrHandler *handler, int val, char *type);
void err_message(ErrHandler *handler);
int read_err_code(ErrHandler *handler);


#endif