#include <stdio.h>
#include "heap_alloc.h"

/**
 * @brief function use to split a free block memory
 *
 * @param size size required by user
 * @param index index of the FREE BLOCK found
 * @return char* a pointer to the first block allocated to user
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
    allocate_first_free_index(size, index, rest);
    heap[index] = size;
    heap[index + 1] = 0;
    ret = heap + index + 1;
    return ret;
}

/**
 * @brief main function that allocate zone
 *
 * @param size sizer required by user
 * @return char* a pointer to the first block allocated to user
 */
char *heap_malloc(unsigned int size)
{
    char *heap;
    t_strategy strategy;
    char *ret;
    int index;

    heap = get_heap();
    strategy = *get_strategy();
    index = (*strategy)(size);
    if (!size)
        return (heap + index + 1);
    ret = NULL;
    if (index != -1)
        ret = fragmentation(size, index);
    return ret;
}

/**
 * @brief Get the previous index
 *
 * @param index current index
 * @return int previous index
 */
int get_previous_index(int index)
{
    char *heap;
    int first_index;
    int previous_index;

    heap = get_heap();
    first_index = get_first_free_index();
    previous_index = get_first_free_index();
    while (first_index < index)
    {
        if (previous_index != first_index)
            previous_index = first_index;
        first_index = heap[first_index + 1];
    }
    return previous_index;
}

/**
 * @brief this function try to try to merge two block selected
 *
 * @param current_index index of the first block
 * @param next_index index of the second block
 */
static void merge(int current_index, int next_index)
{
    char *heap;
    int size;

    heap = get_heap();
    size = (heap + next_index) - (heap + current_index) - 1;
    if (size == heap[current_index])
    {
        heap[current_index] += heap[next_index] + 1;
        heap[current_index + 1] = heap[next_index + 1];
    }
}

/**
 * @brief this function merge right then left free block.
 *
 * @param index index of the current block
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
 * @brief main function that free an allocated zone
 *
 * @param ptr address of memory that need to be free.
 */
void heap_free(char *ptr)
{
    char *heap;
    int first_index;
    int index;
    int previous_index;

    if (!ptr)
        return;
    heap = get_heap();
    first_index = get_first_free_index();
    index = ptr - heap - 1;
    if (index == first_index)
        return;
    if (index < first_index)
    {
        *ptr = first_index;
        set_first_free_index(index);
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
    int first_index;
    int index;
    int size;

    heap = get_heap();
    first_index = get_first_free_index();
    printf("first FREE BLOCK index: %d\n", first_index);
    printf("show heap:\n");
    index = 0;
    while (index < SIZE)
    {
        size = heap[index];
        printf("    index: %d, size = %d, content: %.*s\n", index, size, size, heap + index + 1);
        index += size + 1;
    }
}