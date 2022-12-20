#include "heap_alloc.h"
#include "heap_logger.h"
#include "heap_sem.h"

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
    if (rest > 0)
    {
        t_list new_block;

        new_block = (t_list)((char *)free_block + size + sizeof(*free_block));
        new_block->size = rest;
        replace_elem_by(free_block, new_block);
    }
    else
        remove_elem(free_block);
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
    heap_lock();
    free_block = (*strategy)(size);
    if (!size)
        return NULL;
    ret = NULL;
    if (free_block)
    {
        ret = fragmentation(size, free_block);
        write_log("malloced", free_block->size, ret);
    }
    heap_unlock();
    return ret;
}