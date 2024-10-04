#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "poison.h"

int cat_exec(char *param, Queue *in, Queue *out, int **pilled);

int cat_exec(char *param, Queue *in, Queue *out, int **pilled)
{
    printf("5.1\n");
    if (in != NULL)
        return 4;

    FILE *file = fopen(param, "r");
    if (file == NULL)
        return 6;

    const int STARTER_LENGTH = 50;
    int size = STARTER_LENGTH;

    char *line = malloc(sizeof(char) * size);
    char curr = fgetc(file);
    int cont = 0;
    int i = 0;

    while (cont == 0)
    {
        if (curr != '\n' && curr != EOF)
        {
            if (i == size)
            {
                size = size + (size / 2);
                line = realloc(line, size);
            }
            line[i++] = curr;
        }
        else if (curr == '\n' || curr == EOF)
        {
            char *curr_line = malloc(sizeof(char) * size);
            strcpy(curr_line, line);
            push(out, curr_line);
            free(line);
            i = 0;
            size = STARTER_LENGTH;
            line = malloc(sizeof(char) * size);
        }

        if (curr == EOF)
        {
            cont = 1;
        }
        else
        {
            curr = fgetc(file);
        }
    }
    free(line);

    push(out, POISON_PILL);
    // printf("%s\n", POISON_PILL);
    set_pilled(pilled);

    printf("5.5\n");

    return 0;
}