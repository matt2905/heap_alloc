#ifndef __HEAP_LIST_H__
#define __HEAP_LIST_H__

/**
 * @file heap_list.h
 * @author mmartin
 * @version 0.5
 * @date 2022-12-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stddef.h>

struct s_list
{
    size_t size;
    struct s_list *previous;
    struct s_list *next;
};
typedef struct s_list *t_list;

void prepend(t_list current_block, t_list first_block);
void add_after(t_list current_block, t_list previous_block);
t_list get_previous_block(t_list current_block);
void replace_elem_by(t_list free_block, t_list new_block);
void remove_elem(t_list free_block);

#endif /* __HEAP_LIST_H__ */