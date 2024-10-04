#include <stdlib.h>
#include "poison.h"

const char *POISON_PILL = "POISONPILLabcdefghijklmnopqrstuvwxyz1234567890POISONPILL";

void set_pilled(int **pilled)
{
    int *set_pill = malloc(sizeof(int));
    *set_pill = 1;
    free(*pilled);
    *pilled = set_pill;
}