#include "heap_alloc.h"
#include "heap_list.h"
#include "stdbool.h"

/**
 * @brief add the new element at the start of the list
 *
 * @param new_block
 * @param first_block
 */
void prepend(t_list new_block, t_list first_block)
{
    new_block->next = first_block;
    new_block->previous = NULL;
    first_block->previous = new_block;
    set_first_free_block(new_block);
}

/**
 * @brief add the new element before the selected element
 *
 * @param new_block
 * @param previous_block
 */
void add_after(t_list new_block, t_list previous_block)
{
    new_block->next = previous_block->next;
    new_block->previous = previous_block;
    new_block->next->previous = new_block;
    previous_block->next = new_block;
}

/**
 * @brief Get the previous block
 *
 * @param current_block current block
 * @return t_list previous block
 */
t_list get_previous_block(t_list current_block)
{
    t_list first_block;
    t_list previous_block;

    first_block = get_first_free_block();
    previous_block = get_first_free_block();
    while (current_block - first_block < 0)
    {
        if (previous_block != first_block)
            previous_block = first_block;
        first_block = first_block->next;
    }
    return previous_block;
}

/**
 * @brief replace the selected element by the new one
 *
 * @param free_block
 * @param new_block
 */
void replace_elem_by(t_list free_block, t_list new_block)
{
    new_block->next = free_block->next;
    if (free_block->next)
        free_block->next->previous = new_block;
    new_block->previous = free_block->previous;
    if (free_block->previous)
        free_block->previous->next = new_block;
    if (!new_block->previous)
        set_first_free_block(new_block);
}

/**
 * @brief remove the selected element
 *
 * @param free_block
 */
void remove_elem(t_list free_block)
{
    if (free_block->next)
        free_block->next->previous = free_block->previous;
    if (free_block->previous)
        free_block->previous->next = free_block->next;
    else
        set_first_free_block(free_block->next);
}