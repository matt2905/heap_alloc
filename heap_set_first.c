#include "heap_alloc.h"

/**
 * @brief Set the pointer of first FREE_BLOCK object
 *
 * @param size size required by user
 * @param free_block pointer of the FREE_BLOCK found
 * @param rest free size after fragmentation
 */
#include <stdio.h>
void allocate_first_free_block(size_t size, t_list free_block, int rest)
{
    t_list new_block;
    char *fragmented;

    if (rest > 0)
    {
        fragmented = ((char *)free_block + size + sizeof(*free_block));
        new_block = (t_list)fragmented;
        new_block->next = free_block->next;
        if (free_block->next)
            free_block->next->previous = new_block;
        new_block->previous = free_block->previous;
        if (free_block->previous)
            free_block->previous->next = new_block;
        new_block->size = rest;
        if (!new_block->previous)
            set_first_free_block(new_block);
    }
    else
    {
        if (free_block->next)
            free_block->next->previous = free_block->previous;
        if (free_block->previous)
            free_block->previous->next = free_block->next;
        else
            set_first_free_block(free_block->next);
    }
}