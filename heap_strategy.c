#include <stddef.h>
#include "heap_alloc.h"

/**
 * @brief function to save the best fit index
 *
 * @param size size required by user
 * @param len size of current FREE BLOCK
 * @param index index of current FREE BLOCK
 * @param saved_index index of the best fit FREE BLOCK found
 * @param saved_size size of the best fit FREE BLOCK found
 */
static void best_strategy(unsigned int size, unsigned int len, int index, int *saved_index, unsigned int *saved_size)
{
    if (size <= len && len < *saved_size)
    {
        *saved_index = index;
        *saved_size = len;
    }
}

/**
 * @brief function to save the worst fit index
 *
 * @param size size required by user
 * @param len size of current FREE BLOCK
 * @param index index of current FREE BLOCK
 * @param saved_index index of the worst fit FREE BLOCK found
 * @param saved_size size of the worst fit FREE BLOCK found
 */
static void worst_strategy(unsigned int size, unsigned int len, int index, int *saved_index, unsigned int *saved_size)
{
    if (size <= len && len > *saved_size)
    {
        *saved_index = index;
        *saved_size = len;
    }
}

/**
 * @brief main research function
 *
 * @param size size required by user
 * @param saved_size best or worst size possible depends of strategy
 * @param strategy_type type of strategy used
 * @return int index of the FREE BLOCK found
 */
static int strategy(unsigned int size, unsigned int *saved_size, t_strategy_type strategy_type)
{
    char *heap;
    int saved_index;
    int index;
    unsigned int len;

    heap = get_heap();
    saved_index = -1;
    index = get_first_free_index();
    while (index < SIZE)
    {
        len = (unsigned int)heap[index];
        if (strategy_type == WORST)
            worst_strategy(size, len, index, &saved_index, saved_size);
        else if (strategy_type == BEST)
            best_strategy(size, len, index, &saved_index, saved_size);
        else if (strategy_type == FIRST && size <= len)
            return index;
        if (heap[index + 1] == -1)
            return saved_index;
        index = heap[index + 1];
    }
    return saved_index;
}

/**
 * @brief worst fit search function
 *
 * @param size size required by user
 * @return int worst index found
 */
int worst_fit(unsigned int size)
{
    unsigned int saved_size;

    saved_size = 0;
    return strategy(size, &saved_size, WORST);
}

/**
 * @brief best fit search function
 *
 * @param size size required by user
 * @return int best index found
 */
int best_fit(unsigned int size)
{
    unsigned int saved_size;

    saved_size = SIZE;
    return strategy(size, &saved_size, BEST);
}

/**
 * @brief first fit search function
 *
 * @param size size required by user
 * @return int first index found
 */
int first_fit(unsigned int size)
{
    return strategy(size, NULL, FIRST);
}