#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "error.h"
#include "cat.c"
#include "print.c"
#include "word_count.c"

typedef int (*execute_func)(char *param, Queue *in, Queue *out, int **pilled);

typedef struct Filter
{
    char *type;
    char *param;
    struct Filter *next;
    Queue *in;
    Queue *out;
    int *pilled;
    execute_func execute;

} Filter;

Filter **create_filters(char **sub_cmds, int num);
Filter *make_new(char *sub);
int exec_filt(Filter *cmd, ErrHandler *handler);
execute_func det_filter(char *type, char *param, ErrHandler *handler);

Filter **create_filters(char **sub_cmds, int num)
{
    Filter **filters = malloc(sizeof(Filter *) * num); //
    Filter *curr = NULL;
    Filter *last = NULL;
    for (int i = 0; i < num - 1; i++)
    {
        curr = make_new(sub_cmds[i]);

        if (last != NULL)
        {
            last->next = curr;
            curr->in = last->out;
        }
        last = curr;
        filters[i] = curr;
    }
    Filter *print = make_new("print"); //
    last->next = print;
    filters[num - 1] = print;
    return filters;
}

Filter *make_new(char *sub)
{
    Filter *new = malloc(sizeof(Filter)); //
    int filt = 0;
    int i = 0;
    int swap_idx = 0;

    int t_max = 4;
    char *type = malloc(sizeof(char) * t_max); //
    int t_size = 0;

    int p_max = 10;
    char *param = malloc(sizeof(char) * p_max); //
    int p_size = 0;

    while (sub[i] != '\0')
    {
        // printf("GO %c\n", sub[i]);
        if (filt == 0)
        {
            t_size++;
            if (t_size - 1 == t_max && sub[i] != ' ')
            {
                // printf("ENTER\n");
                t_max *= 2;
                type = realloc(type, sizeof(char) * t_max); //
            }

            if (sub[i] == ' ' && t_size > 1)
            {
                filt = 1;
                type = realloc(type, t_size * sizeof(char)); //
            }
            else if (sub[i] == ' ' && t_size == 1)
            {
                t_size--;
            }
            else
            {
                // printf("CHAR %c\n", sub[i]);
                type[t_size - 1] = sub[i];
            }
        }
        else
        {
            p_size++;
            if (p_size - 1 == p_max)
            {
                p_max *= 2;
                param = realloc(param, sizeof(char) * p_max); //
            }
            else if (sub[i] == ' ')
            {
                p_size--;
            }
            else
            {
                param[p_size - 1] = sub[i];
            }
            param = realloc(param, p_size * sizeof(char)); //
        }
        i++;
    }
    // printf("TYPE %s\n", type);
    new->type = type;
    if (strcmp(param, "") == 0)
        new->param = NULL;
    else
        new->param = param;
    new->out = intialize_queue();
    new->pilled = malloc(sizeof(int)); //
    *(new->pilled) = 0;
    return new;
}

int exec_filt(Filter *cmd, ErrHandler *handler)
{
    printf("1\n");
    char *type = cmd->type;
    char *param = cmd->param;
    int err_code = 0;

    printf("2\n");
    cmd->execute = det_filter(type, param, handler);
    printf("3\n");
    if (cmd->execute)
    {
        printf("4\n");

        while (*(cmd->pilled) == 0 && read_err_code(handler) == 0)
        {
            printf("5\n");
            err_code = cmd->execute(param, cmd->in, cmd->out, &(cmd->pilled));
            printf("6\n");

            if (err_code > 0)
                update_err(handler, err_code, type);
        }

        err_message(handler);
        free(cmd->in);
        if (err_code > 0 || handler->err_code > 0 || cmd->next == NULL)
        {
            free(cmd->out);
        }
        else
        {
            // print_q(cmd->out);
            cmd->next->in = cmd->out;
        }
    }
    return err_code;
}

execute_func det_filter(char *type, char *param, ErrHandler *handler)
{
    int err_code = 0;
    if (strcmp(type, "cat") == 0)
    {
        if (param == NULL)
            err_code = 1;
        else
        {
            return &cat_exec;
        }
    }
    else if (strcmp(type, "pwd") == 0) // for commands that cant take input, check for null
    {
        if (param != NULL)
            err_code = 2;
        else
        {
            printf("pwd\n");
            return NULL;
        }
    }
    else if (strcmp(type, "ls") == 0)
    {
        if (param != NULL)
            err_code = 2;
        else
        {
            printf("ls\n");
            return NULL;
        }
    }
    else if (strcmp(type, "cd") == 0)
    {
        if (param == NULL)
            err_code = 1;
        else
        {
            printf("cd\n");
            return NULL;
        }
    }
    else if (strcmp(type, "head") == 0)
    {
        if (param == NULL)
            err_code = 1;
        else
        {
            printf("head\n");
            return NULL;
        }
    }
    else if (strcmp(type, "tail") == 0)
    {
        if (param == NULL)
            err_code = 1;
        else
        {
            printf("tail\n");
            return NULL;
        }
    }
    else if (strcmp(type, "grep") == 0)
    {
        if (param == NULL)
            err_code = 1;
        else
        {
            printf("grep\n");
            return NULL;
        }
    }
    else if (strcmp(type, "wc") == 0)
    {
        if (param != NULL)
            err_code = 2;
        else
        {
            return &word_count_exec;
        }
    }
    else if (strcmp(type, "uniq") == 0)
    {
        if (param == NULL)
            err_code = 1;
        else
        {
            printf("uniq\n");
            return NULL;
        }
    }
    else if (strcmp(type, ">") == 0)
    {
        if (param == NULL)
            err_code = 1;
        else
        {
            printf(">\n");
            return NULL;
        }
    }
    else if (strcmp(type, "print") == 0)
    {
        return &print_exec;
    }
    else
    {
        err_code = 3;
    }

    if (err_code > 0)
    {
        update_err(handler, err_code, type);
        return NULL;
    }
}