#include "heap_alloc.h"

/**
 * @brief Get the heap object
 *
 * @return char*
 */
void *get_heap()
{
    static char heap[SIZE] = {0};
    t_list first;

    if (heap[0] == 0)
    {
        first = (t_list)heap;
        first->size = (SIZE - sizeof(*first));
        first->next = NULL;
        first->previous = NULL;
    }
    return (void *)heap;
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
 * @brief first free bloc object
 *
 * @return t_strategy*
 */
static t_list *parametized_first_free_block()
{
    static t_list free_block = NULL;

    if (!free_block)
        free_block = get_heap();
    return &free_block;
}

/**
 * @brief Get the first free bloc object
 *
 * @return int
 */
t_list get_first_free_block()
{
    return *parametized_first_free_block();
}

/**
 * @brief Set the first free bloc object
 */
void set_first_free_block(t_list new_free)
{
    t_list *old_free;

    old_free = parametized_first_free_block();
    *old_free = new_free;
}