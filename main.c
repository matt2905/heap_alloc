#include <stdio.h>
#include <string.h>
#include "heap_alloc.h"

/**
 * @brief test given in the doc
 *
 */
void exemple()
{
    char *p1, *p2, *p3, *p4;

    p1 = heap_malloc(10);
    p2 = heap_malloc(9);
    p3 = heap_malloc(5);
    strcpy(p1, "tp 1");
    strcpy(p2, "tp 2");
    strcpy(p3, "tp 3");
    printf("malloc 10, 9, 5, write p1, p2, p3\n");
    show_heap();
    heap_free(p2);
    printf("free p2\n");
    show_heap();
    p4 = heap_malloc(8);
    strcpy(p4, "systeme");
    printf("malloc and write p4\n");
    show_heap();
    heap_free(p1);
    printf("free p1\n");
    show_heap();
    heap_free(p3);
    printf("free p3\n");
    show_heap();
    heap_free(p4);
    printf("free all\n");
    show_heap();
}

/**
 * @brief test to merging memory on free
 *
 */
void defrag_free()
{
    char *p1, *p2, *p3, *p4;

    p1 = heap_malloc(10);
    p2 = heap_malloc(9);
    p3 = heap_malloc(5);
    p4 = heap_malloc(8);
    strcpy(p1, "tp 1");
    strcpy(p2, "tp 2");
    strcpy(p3, "tp 3");
    strcpy(p4, "systeme");
    printf("malloc and write\n");
    show_heap();

    heap_free(p2);
    printf("free p2\n");
    show_heap();
    heap_free(p3);
    printf("free p3\n");
    show_heap();
    heap_free(p4);
    printf("free p4\n");
    show_heap();
    heap_free(p1);
    printf("free p1\n");
    show_heap();
}

/**
 * @brief use the two test for each strategy
 *
 * @return int
 */
int main(void)
{
    printf("\033[0;31m FIRST_FIT strategy: \033[0m\n");
    exemple();
    defrag_free();
    printf("\033[0;31m WORST_FIT strategy: \033[0m\n");
    set_strategy(&worst_fit);
    exemple();
    defrag_free();
    printf("\033[0;31m BEST_FIT strategy: \033[0m\n");
    set_strategy(&best_fit);
    exemple();
    defrag_free();
    return (0);
}