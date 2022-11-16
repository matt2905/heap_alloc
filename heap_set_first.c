#include "heap_alloc.h"

/**
 * @brief this function reallocate the first free block
 *
 * @param size size required by user
 * @param index index of the FREE BLOCK found
 * @param rest free size after fragmentation
 */
static void realloc_first(int size, int index, int rest)
{
    char *heap;
    int *first_index;

    heap = get_heap();
    first_index = get_first_free_index();
    if (rest)
    {
        *first_index = index + size + 1;
        heap[*first_index] = rest;
        heap[*first_index + 1] = heap[index + 1];
    }
    else
        *first_index = heap[index + 1];
}

/**
 * @brief this function is used when the first free block is also the last one
 *
 * @param size size required by user
 * @param index index of the FREE BLOCK found
 * @param rest free size after fragmentation
 */
static void realloc_last(int size, int index, int rest)
{
    char *heap;
    int *first_index;

    heap = get_heap();
    first_index = get_first_free_index();
    *first_index = index + size + 1;
    heap[*first_index] = rest;
    heap[*first_index + 1] = -1;
}

/**
 * @brief This function is used to redefined the pointer to the next free block
 *
 * @param size size required by user
 * @param index index of the FREE BLOCK found
 * @param rest free size after fragmentation
 */
static void realloc_general(int size, int index, int rest)
{
    char *heap;
    int *first_index;

    heap = get_heap();
    first_index = get_first_free_index();
    if (rest)
    {
        heap[index + size + 1] = rest;
        heap[index + size + 2] = heap[index + 1];
        heap[*first_index + 1] = index + size + 1;
    }
    else
        heap[*first_index + 1] = heap[index + 1];
}

/**
 * @brief Set the index of first FREE BLOCK object
 *
 * @param size size required by user
 * @param index index of the FREE BLOCK found
 * @param rest free size after fragmentation
 */
void set_first_free_index(int size, int index, int rest)
{
    char *heap;
    int *first_index;

    heap = get_heap();
    first_index = get_first_free_index();
    if (index == *first_index)
        realloc_first(size, index, rest);
    else if (heap[*first_index + 1] == -1)
        realloc_last(size, index, rest);
    else
        realloc_general(size, index, rest);
}