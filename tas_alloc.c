#include <stddef.h>
#include <stdio.h>

#include "tas_alloc.h"

char *get_tas()
{
    static char tas[SIZE] = {SIZE - 1, -1};

    return tas;
}

t_strategy *get_strategy()
{
    static t_strategy strategy = &first_fit;

    return &strategy;
}

int *get_first_libre()
{
    static int index;

    return &index;
}

char *fragmentation(unsigned taille, int libre)
{
    int rest;
    char *tas;
    char *ret;
    int *first_libre;

    first_libre = get_first_libre();
    tas = get_tas();
    rest = tas[libre] - taille - 1;
    tas[libre] = taille;
    tas[libre + 1] = 0;
    ret = &tas[libre + 1];
    libre += taille + 1;
    if (rest >= 0)
    {
        tas[libre] = rest;
        *first_libre = libre;
    }
    if (rest > 0)
        tas[libre + 1] = -1;
    return ret;
}

char *tas_malloc(unsigned int taille)
{
    t_strategy *strategy;
    char *ret;
    int libre;

    strategy = get_strategy();
    libre = (**strategy)(taille);
    ret = NULL;
    if (libre != -1)
        ret = fragmentation(taille, libre);
    return ret;
}

void defragmentation()
{
    char *tas;
    int *first_libre;
    int savelen;
    int saveindex;
    int increment;

    tas = get_tas();
    first_libre = get_first_libre();
    savelen = 0;
    saveindex = -1;
    for (int index = 0; index < SIZE;)
    {
        increment = tas[index];
        if (tas[index + 1] == -1 || tas[index] == 0)
        {
            if (index < *first_libre)
                *first_libre = index;
            savelen += tas[index];
            if (saveindex == -1)
                saveindex = index;
            else
            {
                savelen += 1;
                tas[saveindex] = savelen;
                tas[index] = -1;
            }
        }
        else
        {
            saveindex = -1;
            savelen = 0;
        }
        index += increment + 1;
    }
}

int tas_free(char *ptr)
{
    if (ptr)
    {
        if (*(ptr - 1) != 0)
            *ptr = -1;
        defragmentation();
    }
    return 0;
}

void show_tas()
{
    char *tas;
    int *first_libre;
    int index;
    int size;

    tas = get_tas();
    first_libre = get_first_libre();
    printf("first bloc libre: %d\n", *first_libre);
    printf("show tas:\n");
    index = 0;
    while (index < SIZE)
    {
        size = tas[index];
        printf("    index: %d, size = %d, content: %.*s\n", index, size, size, tas + index + 1);
        index += size + 1;
    }
}