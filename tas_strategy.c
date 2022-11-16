#include <stddef.h>
#include "tas_alloc.h"

static void best_strategy(unsigned int size, unsigned int len, int index, int *saved_index, unsigned int *saved_size)
{
    if (size <= len && len < *saved_size)
    {
        *saved_index = index;
        *saved_size = len;
    }
}

static void worst_strategy(unsigned int size, unsigned int len, int index, int *saved_index, unsigned int *saved_size)
{
    if (size <= len && len > *saved_size)
    {
        *saved_index = index;
        *saved_size = len;
    }
}

static int strategy(unsigned int size, unsigned int *saved_size, t_strategy_type strategy_type)
{
    char *tas;
    int saved_index;
    int index;
    unsigned int len;

    tas = get_tas();
    saved_index = -1;
    index = *get_first_libre();
    while (index < SIZE)
    {
        len = (unsigned int)tas[index];
        if (strategy_type == WORST)
            worst_strategy(size, len, index, &saved_index, saved_size);
        else if (strategy_type == BEST)
            best_strategy(size, len, index, &saved_index, saved_size);
        else if (strategy_type == FIRST && size <= len)
            return index;
        if (tas[index + 1] == -1)
            return saved_index;
        index = tas[index + 1];
    }
    return saved_index;
}

int worst_fit(unsigned int size)
{
    unsigned int saved_size;

    saved_size = 0;
    return strategy(size, &saved_size, WORST);
}

int best_fit(unsigned int size)
{
    unsigned int saved_size;

    saved_size = SIZE;
    return strategy(size, &saved_size, BEST);
}

int first_fit(unsigned int size)
{
    return strategy(size, NULL, FIRST);
}