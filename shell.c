#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "filter.c"
#include "queue.h"
#include "error.h"

typedef struct exec_params
{
    Filter *cmd;
    ErrHandler *handler;
} 
exec_params;

// GO BACK AND INCLUDE ERROR HANDLING FOR MEMORY ALLOCATION WHEN NO MEMORY REMAINING

char **grab_cmds(int **run, int **num_sub);
void print_sub(Filter **sub, int num);
void *run_cmd(void *params);

int main(void)
{
    int *run = malloc(sizeof(int)); //
    *run = 0;
    int *num_sub = malloc(sizeof(int)); //
    *num_sub = 0;
    char **sub_cmds;
    ErrHandler *handler = malloc(sizeof(ErrHandler));
    initialize(handler);

    while (*run == 0)
    {
        sub_cmds = grab_cmds(&run, &num_sub);
        if (*run == 1)
            break;

        int sub = (*num_sub) + 1;
        Filter **filters = create_filters(sub_cmds, sub);
        free(sub_cmds);

        pthread_t *threads = malloc(sizeof(pthread_t) * sub);
        for (int i = 0; i < sub; i++)
        {
            exec_params *params = malloc(sizeof(exec_params *));
            params->cmd = filters[i];
            params->handler = handler;
            pthread_create(&threads[i], NULL, run_cmd, params);
        }
        pthread_join(threads[sub-1], NULL);
        // print_sub(filters, sub);
    }
    free(run);
    free(num_sub);
    free(handler);
}

char **grab_cmds(int **run, int **num_sub)
{
    int size = 20;
    char *cmd = malloc(sizeof(char) * size); //
    char curr;
    int idx = 0;

    int max_sub = 5;
    char **sub_cmds = malloc(sizeof(char *) * max_sub); //
    int *num_cmds = malloc(sizeof(int));                //
    int cmd_count = 0;
    printf("> ");
    while ((curr = getchar()) != '\n' && curr != EOF)
    {
        if (curr != '|')
            cmd[idx] = curr;
        else
            cmd[idx] = '\0';

        idx++;
        if (idx == size - 1 && curr != '|')
        {
            size *= 2;
            cmd = realloc(cmd, (size) * sizeof(char)); //
        }
        else if (strcmp(cmd, "exit") == 0)
        {
            int *end = malloc(sizeof(int)); //
            free(*run);
            *end = 1;
            *run = end;
            break;
        }
        else if (curr == '|')
        {
            sub_cmds[cmd_count] = realloc(cmd, idx * sizeof(char)); //
            size = 20;
            cmd = malloc(sizeof(char) * size); //
            idx = 0;
            cmd_count++;

            if (cmd_count == max_sub)
            {
                max_sub *= 2;
                sub_cmds = realloc(sub_cmds, max_sub * sizeof(char *)); //
            }
        }
    }
    cmd[idx] = '\0';
    sub_cmds[cmd_count] = realloc(cmd, idx * sizeof(char)); //
    cmd_count++;
    sub_cmds = realloc(sub_cmds, cmd_count * sizeof(char *)); //
    *num_sub = &cmd_count;

    return sub_cmds;
}

void *run_cmd(void *params)
{
    exec_params *ex_pars = ((exec_params *)params);
    exec_filt(ex_pars->cmd, ex_pars->handler);
}

void print_sub(Filter **sub, int num)
{
    for (int i = 0; i < num; i++)
    {
        printf("Filter type: %s\tparam: %s\t", sub[i]->type, sub[i]->param);
        // printf("Filter type: %s\tparam: %s\t", (*(sub[i])).type, (*(sub[i])).param);

        if (sub[i]->next != NULL)
        {
            printf("next: %s", sub[i]->next->type);
        }
        printf("\n");
    }
    // printf("\n");
}