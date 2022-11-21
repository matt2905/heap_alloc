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
 * @brief strategy object
 *
 * @return t_strategy*
 */
static t_strategy *parametized_strategy()
{
    static t_strategy strategy = &first_fit;

    return &strategy;
}

/**
 * @brief Get the strategy object
 *
 * @return t_strategy
 */
t_strategy get_strategy()
{
    return *parametized_strategy();
}

/**
 * @brief Set the strategy object
 */
void set_strategy(t_strategy new_strategy)
{
    t_strategy *old_strategy;

    old_strategy = parametized_strategy();
    *old_strategy = new_strategy;
}

/**
 * @brief first free index object
 *
 * @return t_strategy*
 */
static int *parametized_first_free_index()
{
    static int index;

    return &index;
}

/**
 * @brief Get the first free index object
 *
 * @return int
 */
int get_first_free_index()
{
    return *parametized_first_free_index();
}

/**
 * @brief Set the first free index object
 */
void set_first_free_index(int new_index)
{
    int *old_index;

    old_index = parametized_first_free_index();
    *old_index = new_index;
}