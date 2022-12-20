#include <stdio.h>
#include "heap_alloc.h"

static void show_free_block_index(void)
{
    char *heap;
    t_list free_block;
    int index;

    heap = (char *)(get_heap());
    free_block = get_first_free_block();

    printf("FREE BLOCK index: \n");
    while (free_block)
    {
        index = (char *)free_block - heap;
        printf("    index: %d, size: %ld\n", index, free_block->size);
        free_block = free_block->next;
    }
}

/**
 * @brief function to print the heap
 *
 */
void show_heap()
{
    char *heap;
    char *content;
    int index;
    int size;

    heap = (char *)(get_heap());
    show_free_block_index();
    printf("show heap:\n");
    index = 0;
    while (index < SIZE)
    {
        size = ((t_list)(heap + index))->size;
        content = heap + index + sizeof(struct s_list);
        printf("    index: %d, size = %d, content: %s\n", index, size, content);
        index += size + sizeof(struct s_list);
    }
}