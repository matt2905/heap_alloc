#include "tas_alloc.h"

/**
 * @brief this function allocate the first free block
 *        and move the first index of free block to the next one
 *
 * @param size
 * @param index
 * @param rest
 */
static void realloc_first(int size, int index, int rest)
{
    char *tas;
    int *first_index;

    tas = get_tas();
    first_index = get_first_libre();
    if (rest)
    {
        *first_index = index + size + 1;
        tas[*first_index] = rest;
        tas[*first_index + 1] = tas[index + 1];
    }
    else
        *first_index = tas[index + 1];
}

/**
 * @brief this function is used when the first free block is also the last one
 *
 * @param size
 * @param index
 * @param rest
 */
static void realloc_last(int size, int index, int rest)
{
    char *tas;
    int *first_index;

    tas = get_tas();
    first_index = get_first_libre();
    *first_index = index + size + 1;
    tas[*first_index] = rest;
    tas[*first_index + 1] = -1;
}

/**
 * @brief This function is used to redefined the pointer to the next free block
 *
 * @param size
 * @param index
 * @param rest
 */
static void realloc_general(int size, int index, int rest)
{
    char *tas;
    int *first_index;

    tas = get_tas();
    first_index = get_first_libre();
    if (rest)
    {
        tas[index + size + 1] = rest;
        tas[index + size + 2] = tas[index + 1];
        tas[*first_index + 1] = index + size + 1;
    }
    else
        tas[*first_index + 1] = tas[index + 1];
}

void set_first_libre(int size, int index, int rest)
{
    char *tas;
    int *first_index;

    tas = get_tas();
    first_index = get_first_libre();
    if (index == *first_index)
        realloc_first(size, index, rest);
    else if (tas[*first_index + 1] == -1)
        realloc_last(size, index, rest);
    else
        realloc_general(size, index, rest);
}