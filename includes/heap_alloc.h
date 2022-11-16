/**
 * @file heap_alloc.h
 * @author mmartin
 * @version 0.1
 * @date 2022-11-16
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef HEAP_ALLOC_H
#define HEAP_ALLOC_H

/**
 * @def SIZE
 * @brief Define the size max of the heap
 */
#define SIZE 128
/**
 * @def FREE_BLOCK
 * @brief Define the last free block
 */
#define FREE_BLOCK -1

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

typedef int (*t_strategy)(unsigned int size);
t_strategy *get_strategy();
int *get_first_libre();

char *get_heap();
char *heap_malloc(unsigned int size);
void heap_free(char *ptr);
void show_heap();

int first_fit(unsigned int size);
int best_fit(unsigned int size);
int worst_fit(unsigned int size);

void set_first_libre(int size, int index, int rest);

#endif