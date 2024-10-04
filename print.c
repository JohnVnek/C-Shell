#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "poison.h"

int print_exec(char *param, Queue *in, Queue *out, int **pilled);

int print_exec(char *param, Queue *in, Queue *out, int **pilled)
{
    // print_q(in);

    const char *curr;
    while ((curr = poll(in)) != NULL)
    {
        if (strcmp(curr, POISON_PILL) == 0)
        {
            set_pilled(pilled);
        }
        else
        {
            printf("%s\n", curr);
        }
    }
    return 0;
}