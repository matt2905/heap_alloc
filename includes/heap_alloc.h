#ifndef HEAP_ALLOC_H
#define HEAP_ALLOC_H

/**
 * @file heap_alloc.h
 * @author mmartin
 * @version 0.5
 * @date 2022-11-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "heap_list.h"

/**
 * @def SIZE
 * @brief Define the size max of the heap
 */
#define SIZE 1024

/**
 * @enum t_strategy_type
 * @brief Use to select the chossen strategy
 *
 * @var t_strategy_type FIRST
 * @brief First fit strategy
 *
 * @var t_strategy_type BEST
 * @brief Best fit strategy
 *
 * @var t_strategy_type WORST
 * @brief worst fit strategy
 */
typedef enum e_strategy_type
{
	FIRST,
	BEST,
	WORST
} t_strategy_type;

/**
 * @brief Function pointer for use the chossen strategy
 *
 */
typedef t_list (*t_strategy)(size_t size);

t_strategy get_strategy();
void set_strategy(t_strategy new_strategy);
t_list get_first_free_block();
void set_first_free_block(t_list new_free);

void *get_heap();
void *heap_malloc(size_t size);
void heap_free(void *ptr);
void show_heap();

t_list first_fit(size_t size);
t_list best_fit(size_t size);
t_list worst_fit(size_t size);

#endif