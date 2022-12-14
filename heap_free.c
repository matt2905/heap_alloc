#include "heap_alloc.h"

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
 * @brief this function try to try to merge two block selected
 *
 * @param current_block pointer of the first block
 * @param next_block pointer of the second block
 */
static void merge(t_list current_block, t_list next_block)
{
    size_t size;

    size = (char *)next_block - (char *)current_block - sizeof(*current_block);
    if (size == current_block->size)
    {
        current_block->size += next_block->size + sizeof(*next_block);
        current_block->next = next_block->next;
    }
}

/**
 * @brief this function merge right then left free block.
 *
 * @param index pointer of the current block
 */
void defragmentation(t_list current_block)
{
    t_list previous_block;
    t_list next_block;

    previous_block = get_previous_block(current_block);
    next_block = current_block->next;
    merge(current_block, next_block);
    merge(previous_block, current_block);
}

/**
 * @brief main function that free an allocated zone
 *
 * @param ptr address of memory that need to be free.
 */
void heap_free(void *ptr)
{
    char *current;
    t_list current_block;
    t_list first_block;
    t_list previous_block;

    if (!ptr)
        return;
    current = (char *)ptr - sizeof(*current_block);
    current_block = (t_list)current;
    *(char *)ptr = -1;
    first_block = get_first_free_block();
    if (current_block == first_block)
        return;
    if (current_block - first_block < 0)
    {
        current_block->next = first_block;
        current_block->previous = NULL;
        first_block->previous = current_block;
        set_first_free_block(current_block);
    }
    else
    {
        previous_block = get_previous_block(current_block);
        current_block->next = previous_block->next;
        current_block->previous = previous_block;
        current_block->next->previous = current_block;
        previous_block->next = current_block;
    }
    defragmentation(current_block);
}