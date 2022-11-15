#include <stddef.h>
#include <stdio.h>

#include "tas_alloc.h"

char *get_tas()
{
    static char tas[SIZE] = {SIZE - 1, FREE_BLOCK};

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

void set_first_libre()
{
    char *tas;
    int *first_index;

    tas = get_tas();
    first_index = get_first_libre();
    while (tas[*first_index + 1] != FREE_BLOCK)
    {
        *first_index += tas[*first_index] + 1;
    }
}

char *fragmentation(unsigned taille, int libre)
{
    int rest;
    char *tas;
    char *ret;

    tas = get_tas();
    rest = tas[libre] - taille - 1;
    if (!rest)
        taille++;
    tas[libre] = taille;
    tas[libre + 1] = 0;
    ret = &tas[libre + 1];
    libre += taille + 1;
    if (rest > 0)
    {
        tas[libre] = rest;
        tas[libre + 1] = FREE_BLOCK;
    }
    set_first_libre();
    return ret;
}

char *tas_malloc(unsigned int taille)
{
    char *tas;
    t_strategy strategy;
    char *ret;
    int libre;

    tas = get_tas();
    strategy = *get_strategy();
    libre = (*strategy)(taille);
    if (!taille)
        return (&tas[libre + 1]);
    ret = NULL;
    if (libre != -1)
        ret = fragmentation(taille, libre);
    return ret;
}

static void reset(int *savelen, int *saveindex)
{
    *savelen = 0;
    *saveindex = -1;
}

void defragmentation()
{
    char *tas;
    int *first_libre;
    int savelen;
    int saveindex;

    tas = get_tas();
    first_libre = get_first_libre();
    reset(&savelen, &saveindex);
    for (int index = 0; index < SIZE;)
    {
        if (tas[index + 1] == -1)
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
            }
        }
        else
            reset(&savelen, &saveindex);
        index += tas[index] + 1;
    }
}

int tas_free(char *ptr)
{
    if (ptr)
    {
        if (*(ptr - 1) != 0)
            *ptr = FREE_BLOCK;
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