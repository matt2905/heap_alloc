#include "heap_alloc.h"

/**
 * @brief function to save the best fit pointer
 *
 * @param size size required by user
 * @param free_block pointer of current FREE_BLOCK
 * @param saved_block pointer of the best fit FREE_BLOCK found
 */
static void best_strategy(size_t size, t_list free_block, t_list *saved_block)
{
    size_t len;
    size_t saved_size;

    if (*saved_block)
        saved_size = (*saved_block)->size;
    else
        saved_size = SIZE;
    len = free_block->size;
    if (size <= len && len < saved_size)
        *saved_block = free_block;
}

/**
 * @brief function to save the worst fit pointer
 *
 * @param size size required by user
 * @param free_block pointer of the current FREE_BLOCK
 * @param saved_block pointer of the worst fit FREE_BLOCK found
 */
static void worst_strategy(size_t size, t_list free_block, t_list *saved_block)
{
    size_t len;
    size_t saved_size;

    if (*saved_block)
        saved_size = (*saved_block)->size;
    else
        saved_size = 0;
    len = free_block->size;
    if (size <= len && len > saved_size)
        *saved_block = free_block;
}

/**
 * @brief main research function
 *
 * @param size size required by user
 * @param strategy_type type of strategy used
 * @return t_list pointer of the FREE_BLOCK found
 */
static t_list strategy(size_t size, t_strategy_type strategy_type)
{
    t_list saved_block;
    t_list free_block;

    saved_block = NULL;
    free_block = get_first_free_block();
    while (free_block)
    {
        if (strategy_type == WORST)
            worst_strategy(size, free_block, &saved_block);
        else if (strategy_type == BEST)
            best_strategy(size, free_block, &saved_block);
        else if (strategy_type == FIRST && size <= free_block->size)
            return free_block;
        if (free_block->next == NULL)
            return saved_block;
        free_block = free_block->next;
    }
    return saved_block;
}

/**
 * @brief worst fit search function
 *
 * @param size size required by user
 * @return t_list worst FREE_BLOCK found
 */
t_list worst_fit(size_t size)
{
    return strategy(size, WORST);
}

/**
 * @brief best fit search function
 *
 * @param size size required by user
 * @return t_list best FREE_BLOCK found
 */
t_list best_fit(size_t size)
{
    return strategy(size, BEST);
}

/**
 * @brief first fit search function
 *
 * @param size size required by user
 * @return t_list first FREE_BLOCK found
 */
t_list first_fit(size_t size)
{
    return strategy(size, FIRST);
}