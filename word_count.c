// #include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"
#include "poison.h"

int word_count_exec(char *param, Queue *in, Queue *out, int **pilled);
int *count(const char *line);

int word_count_exec(char *param, Queue *in, Queue *out, int **pilled)
{
    if (size(in) == 0)
    {
        return 7;
    }

    // print_q(in);

    int words = 0;
    int chars = 0;
    int lines = size(in);

    char *counter = malloc(sizeof(char) * 20);

    const char *line = NULL;
    int *counts = NULL;
    while ((line = poll(in)) != NULL)
    {
        if (strcmp(line, POISON_PILL) == 0)
        {
            set_pilled(pilled);
        }
        else
        {
            counts = count(line);
            words += counts[0];
            chars += counts[1];
            free(counts);
        }
    }

    sprintf(counter, "%d %d %d", lines, words, chars);

    push(out, counter);

    return 0;
}

int *count(const char *line)
{
    int start_end = 0;
    int words = 0;
    int chars = 0;
    int *counts = malloc(sizeof(int) * 2);

    int i = 0;
    while (line[i] != '\0')
    {
        if (line[i] != '\n' && line[i] != '\t')
            chars++;

        if (start_end == 0 && line[i] != ' ' && line[i] != '\n' && line[i] != '\t')
        {
            start_end = 1;
            words++;
        }
        else if (start_end == 1 && (line[i] == ' ' || line[i] == '\n' || line[i] == '\t'))
        {
            start_end = 0;
        }
        i++;
    }

    counts[0] = words;
    counts[1] = chars;

    return counts;
}
