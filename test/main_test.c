#include "heap_alloc.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <ctype.h>

/**
 * @brief another method to print the heap
 *
 */
void print_heap(void)
{
    char *heap = get_heap();
    int index = get_first_free_index();

    printf("index = %d\n", index);
    int i, j;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 16; j++)
        {
            printf("%4d", j + 16 * i);
        }
        printf("\n");
        for (j = 0; j < 16; j++)
        {
            printf("%4d", heap[j + 16 * i]);
        }
        printf("\n");
        for (j = 0; j < 16; j++)
        {
            if (isprint(heap[j + 16 * i]))
            {
                printf("%4c", heap[j + 16 * i]);
            }
            else
            {
                printf("    ");
            }
        }
        printf("\n\n");
    }
    printf("---------------------------------------------------------------\n\n");
}

static void reset_heap()
{
    char *heap = get_heap();

    bzero(heap, SIZE);
    heap[0] = SIZE - 1;
    heap[1] = FREE_BLOCK;
    set_first_free_index(0);
    set_strategy(&first_fit);
}

/**
 * @brief test with CUNIT of the first part example given in the doc
 *
 */
void test_heap_malloc_example()
{
    char *heap = get_heap();
    char *p1 = heap_malloc(10);
    int index = get_first_free_index();
    strcpy(p1, "tp 1");
    // show_heap();
    // print_heap();

    CU_ASSERT(p1 - 1 == heap);
    CU_ASSERT(*p1 != FREE_BLOCK);

    CU_ASSERT(index == 10 + 1);
    CU_ASSERT(*(heap + index) == 116);
    CU_ASSERT(*(heap + index + 1) == FREE_BLOCK);

    char *p2 = heap_malloc(9);
    // show_heap();
    // print_heap();

    index = get_first_free_index();
    CU_ASSERT(p2 == heap + 12);
    CU_ASSERT(*(p2 - 1) == 9);
    CU_ASSERT(index == 21);

    char *p3 = heap_malloc(5);
    // show_heap();
    // print_heap();

    index = get_first_free_index();
    CU_ASSERT(p3 == heap + 22);
    CU_ASSERT(*(p3 - 1) == 5);
    CU_ASSERT(index == 27);

    char *p4 = heap_malloc(101);
    // show_heap();
    // print_heap();

    CU_ASSERT(p4 == NULL);

    reset_heap();
}

/**
 * @brief test with CUNIT of the merging memory right and left
 *
 */
void test_heap_free_several()
{
    char *heap = get_heap();
    int index;

    char *p1 = heap_malloc(10);
    char *p2 = heap_malloc(10);
    char *p3 = heap_malloc(10);
    char *p4 = heap_malloc(10);
    char *p5 = heap_malloc(10);

    strcpy(p1, "tp1");
    strcpy(p2, "tp2");
    strcpy(p3, "tp3");
    strcpy(p4, "tp4");
    strcpy(p5, "tp5");

    heap_free(p2); // simple free

    index = get_first_free_index();
    CU_ASSERT(*(p2 - 1) == 10);
    CU_ASSERT(*(p2) == 55);
    CU_ASSERT(index == p2 - 1 - heap);

    heap_free(p4); // next free

    index = get_first_free_index();
    CU_ASSERT(*(p2) == 33);
    CU_ASSERT(*(p4) == 55);
    CU_ASSERT(index == p2 - 1 - heap);

    heap_free(p3); // testing merge left

    index = get_first_free_index();
    CU_ASSERT(*(p2 - 1) == 32);
    CU_ASSERT(index == p2 - 1 - heap);

    heap_free(p1); // testing merge right

    index = get_first_free_index();
    CU_ASSERT(*(p1 - 1) == 43);
    CU_ASSERT(*(p1) == 55);
    CU_ASSERT(index == 0);

    reset_heap();
}

/**
 * @brief test with CUNIT of the example given in the doc
 *
 */
void test_full_example()
{
    char *heap = get_heap();
    int index;

    char *p1 = heap_malloc(10);
    char *p2 = heap_malloc(9);
    char *p3 = heap_malloc(5);

    strcpy(p1, "tp 1");
    strcpy(p2, "tp 2");
    strcpy(p3, "tp 3");

    heap_free(p2);

    // That verifies that in case we have 1 byte left we actually malloc size+1
    char *p4 = heap_malloc(8);
    strcpy(p4, "systeme");

    print_heap();

    index = get_first_free_index();
    CU_ASSERT(*(p1 - 1) == 10);
    CU_ASSERT(*(p4 - 1) == 9);
    CU_ASSERT(*(p3 - 1) == 5);
    CU_ASSERT(index == 27);
    CU_ASSERT(heap[27] == 100);
    CU_ASSERT(heap[28] == -1);

    CU_ASSERT(strcmp(p1, "tp 1") == 0);
    CU_ASSERT(strcmp(p3, "tp 3") == 0);
    CU_ASSERT(strcmp(p4, "systeme") == 0);

    reset_heap();
}

/**
 * @brief test with CUNIT of some random allocation
 *
 */
void test_heap_malloc(void)
{
    char *p1, *p2, *p3, *p4;

    p1 = (char *)heap_malloc(10);
    strcpy(p1, "tp 1");
    CU_ASSERT(strcmp(p1, "tp 1") == 0);

    p2 = (char *)heap_malloc(9);
    strcpy(p2, "tp 2");
    CU_ASSERT(strcmp(p2, "tp 2") == 0);

    p3 = (char *)heap_malloc(5);
    strcpy(p3, "tp 3");
    CU_ASSERT(strcmp(p3, "tp 3") == 0);

    p4 = (char *)heap_malloc(200);
    CU_ASSERT(p4 == NULL);

    p1 = (char *)heap_malloc(1);
    strcpy(p1, "\0");
    CU_ASSERT(strcmp(p1, "") == 0);

    reset_heap();
}

/**
 * @brief test with CUNIT of best fit strategy
 *
 */
void test_best_malloc()
{
    set_strategy(&best_fit);

    char *p1 = heap_malloc(20);
    char *p2 = heap_malloc(10);
    char *p3 = heap_malloc(20);
    char *p4 = heap_malloc(8);
    char *p5 = heap_malloc(10);

    strcpy(p1, "tp 1");
    strcpy(p2, "tp 2");
    strcpy(p3, "tp 3");
    strcpy(p4, "tp 4");
    strcpy(p5, "tp 5");
    heap_free(p2);
    heap_free(p4);
    p2 = heap_malloc(5);
    strcpy(p2, "tp 2");

    CU_ASSERT(p2 == p4);
    CU_ASSERT(*(p2 - 1) == 5);

    reset_heap();
}

/**
 * @brief test with CUNIT of worst fit strategy
 *
 */
void test_worst_malloc()
{
    set_strategy(&worst_fit);

    char *p1 = heap_malloc(20);
    char *p2 = heap_malloc(10);
    char *p3 = heap_malloc(20);
    char *p4 = heap_malloc(8);
    char *p5 = heap_malloc(10);

    strcpy(p1, "tp 1");
    strcpy(p2, "tp 2");
    strcpy(p3, "tp 3");
    strcpy(p4, "tp 4");
    strcpy(p5, "tp 5");
    heap_free(p2);
    heap_free(p4);
    p2 = heap_malloc(5);
    strcpy(p2, "tp 2");

    CU_ASSERT(p2 == p5 + 11);
    CU_ASSERT(*(p2 - 1) == 5);

    reset_heap();
}

static int init_suite(void) { return 0; }
static int clean_suite(void) { return 0; }

/**
 * @brief program test with CUNIT
 *
 */
int main(void)
{
    CU_pSuite pSuite = NULL;
    CU_pSuite pSuite2 = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite 1", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    pSuite2 = CU_add_suite("Suite yan", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if (
        NULL == CU_add_test(pSuite2, "of test_heap_malloc_example()", test_heap_malloc_example) ||
        NULL == CU_add_test(pSuite2, "of test_heap_free_several()", test_heap_free_several) ||
        NULL == CU_add_test(pSuite2, "of test_full_example()", test_full_example))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (
        NULL == CU_add_test(pSuite, "of test_heap_malloc()", test_heap_malloc) ||
        NULL == CU_add_test(pSuite, "of test_best_malloc()", test_best_malloc) ||
        NULL == CU_add_test(pSuite, "of test_worst_malloc()", test_worst_malloc))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}