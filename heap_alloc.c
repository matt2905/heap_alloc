#include <stdio.h>
#include "heap_alloc.h"

/**
 * @brief Get the heap object
 *
 * @return char*
 */
char *get_heap()
{
    static char heap[SIZE] = {SIZE - 1, FREE_BLOCK};

    return heap;
}

/**
 * @brief Get the strategy object
 *
 * @return t_strategy*
 */
t_strategy *get_strategy()
{
    static t_strategy strategy = &first_fit;

    return &strategy;
}

/**
 * @brief Get the first libre object
 *
 * @return int*
 */
int *get_first_libre()
{
    static int index;

    return &index;
}

/**
 * @brief function use to split a free block memory
 *
 * @param size
 * @param index
 * @return char*
 */
char *fragmentation(unsigned size, int index)
{
    int rest;
    char *heap;
    char *ret;

    heap = get_heap();
    rest = heap[index] - size - 1;
    if (!rest)
        size++;
    set_first_libre(size, index, rest);
    heap[index] = size;
    heap[index + 1] = 0;
    ret = heap + index + 1;
    return ret;
}

/**
 * @brief main function that allocate zone
 *
 * @param size
 * @return char*
 */
char *heap_malloc(unsigned int size)
{
    char *heap;
    t_strategy strategy;
    char *ret;
    int libre;

    heap = get_heap();
    strategy = *get_strategy();
    libre = (*strategy)(size);
    if (!size)
        return (heap + libre + 1);
    ret = NULL;
    if (libre != -1)
        ret = fragmentation(size, libre);
    return ret;
}

/**
 * @brief Get the previous index
 *
 * @param index
 * @return int
 */
int get_previous_index(int index)
{
    char *heap;
    int first_libre;
    int previous_libre;

    heap = get_heap();
    first_libre = *get_first_libre();
    previous_libre = *get_first_libre();
    while (first_libre < index)
    {
        if (previous_libre != first_libre)
        {
            previous_libre = first_libre;
        }
        first_libre = heap[first_libre + 1];
    }
    return previous_libre;
}

/**
 * @brief this function try to try to merge two block selected
 *
 * @param previous_index
 * @param next_index
 */
static void merge(int previous_index, int next_index)
{
    char *heap;
    int size;

    heap = get_heap();
    size = (heap + next_index) - (heap + previous_index) - 1;
    if (size == heap[previous_index])
    {
        heap[previous_index] += heap[next_index] + 1;
        heap[previous_index + 1] = heap[next_index + 1];
    }
}

/**
 * @brief this function merge right then left free block.
 *
 * @param index
 */
void defragmentation(int index)
{
    char *heap;
    int previous_index;
    int next_index;

    heap = get_heap();
    previous_index = get_previous_index(index);
    next_index = heap[index + 1];
    merge(index, next_index);
    merge(previous_index, index);
}

/**
 * @brief this function add at address +1, the index of the next free block
 *        and save the first free block.
 *
 * @param ptr, address of memory that need to be free.
 */
void heap_free(char *ptr)
{
    char *heap;
    int *first_libre;
    int index;
    int previous_index;

    if (!ptr)
        return;
    heap = get_heap();
    first_libre = get_first_libre();
    index = ptr - heap - 1;
    if (index == *first_libre)
        return;
    if (index < *first_libre)
    {
        *ptr = *first_libre;
        *first_libre = index;
    }
    else
    {
        previous_index = get_previous_index(index);
        *ptr = heap[previous_index + 1];
        heap[previous_index + 1] = index;
    }
    defragmentation(index);
}

/**
 * @brief function to print the heap
 *
 */
void show_heap()
{
    char *heap;
    int *first_libre;
    int index;
    int size;

    heap = get_heap();
    first_libre = get_first_libre();
    printf("first bloc libre: %d\n", *first_libre);
    printf("show heap:\n");
    index = 0;
    while (index < SIZE)
    {
        size = heap[index];
        printf("    index: %d, size = %d, content: %.*s\n", index, size, size, heap + index + 1);
        index += size + 1;
    }
}