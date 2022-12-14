#include "heap_alloc.h"

/**
 * @brief function use to split a free block memory
 *
 * @param size size required by user
 * @param index index of the FREE BLOCK found
 * @return void* a pointer to the first block allocated to user
 */
void *fragmentation(size_t size, t_list free_block)
{
    int rest;
    t_list current_block;
    char *ret;

    current_block = (void *)free_block;
    rest = free_block->size - size - sizeof(*free_block);
    if (rest <= 0)
        size = free_block->size;
    allocate_first_free_block(size, free_block, rest);
    current_block->size = size;
    ret = (char *)current_block + sizeof(*current_block);
    return (void *)ret;
}

/**
 * @brief main function that allocate zone
 *
 * @param size sizer required by user
 * @return char* a pointer to the first block allocated to user
 */
void *heap_malloc(size_t size)
{
    t_strategy strategy;
    void *ret;
    t_list free_block;

    strategy = *get_strategy();
    free_block = (*strategy)(size);
    if (!size)
        return NULL;
    ret = NULL;
    if (free_block)
        ret = fragmentation(size, free_block);
    return ret;
}