#include <stdio.h>
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

/**
 * @brief function to print the heap
 *
 */
void show_heap()
{
    char *first_index;
    char *ptr;
    char *content;
    int index;
    int size;

    ptr = (char *)(get_heap());
    first_index = (char *)get_first_free_block();
    printf("first FREE BLOCK index: %ld\n", first_index - ptr);
    printf("show heap:\n");
    index = 0;
    while (index < SIZE)
    {
        size = ((t_list)(ptr + index))->size;
        content = (char *)((ptr + index) + sizeof(struct s_list));
        printf("    index: %d, size = %d, content: %.*s\n", index, size, size, content);
        index += size + sizeof(struct s_list);
    }
}