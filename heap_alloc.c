#include <stdio.h>
#include "heap_alloc.h"

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