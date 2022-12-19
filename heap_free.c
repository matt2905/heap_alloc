#include "heap_alloc.h"
#include "heap_sem.h"

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
    t_list current_block;
    t_list first_block;
    t_list previous_block;

    if (!ptr)
        return;
    heap_lock();
    current_block = (t_list)((char *)ptr - sizeof(*current_block));
    *(char *)ptr = -1;
    first_block = get_first_free_block();
    if (current_block == first_block)
        return;
    if (current_block - first_block < 0)
        prepend(current_block, first_block);
    else
    {
        previous_block = get_previous_block(current_block);
        add_after(current_block, previous_block);
    }
    defragmentation(current_block);
    heap_unlock();
}