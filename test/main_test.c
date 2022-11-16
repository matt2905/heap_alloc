#include "tas_alloc.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <ctype.h>

void print_heap(void)
{
    char *heap = get_tas();
    int libre = *get_first_libre();

    printf("libre = %d\n", libre);
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
    char *heap = get_tas();
    int *first_libre = get_first_libre();

    bzero(heap, SIZE);
    heap[0] = SIZE - 1;
    heap[1] = FREE_BLOCK;
    *first_libre = 0;
}

void test_tas_malloc(void)
{
    char *p1, *p2, *p3, *p4;

    p1 = (char *)tas_malloc(10);
    strcpy(p1, "tp 1");
    CU_ASSERT(strcmp(p1, "tp 1") == 0);

    p2 = (char *)tas_malloc(9);
    strcpy(p2, "tp 2");
    CU_ASSERT(strcmp(p2, "tp 2") == 0);

    p3 = (char *)tas_malloc(5);
    strcpy(p3, "tp 3");
    CU_ASSERT(strcmp(p3, "tp 3") == 0);

    p4 = (char *)tas_malloc(200);
    CU_ASSERT(p4 == NULL);

    reset_heap();
}

void test_tas_malloc_empty(void)
{
    char *p1;

    p1 = (char *)tas_malloc(1);
    strcpy(p1, "\0");
    CU_ASSERT(strcmp(p1, "") == 0);

    reset_heap();
}

void test_heap_malloc_example()
{
    char *heap = get_tas();
    char *p1 = tas_malloc(10);
    int libre = *get_first_libre();
    strcpy(p1, "tp 1");
    // show_tas();
    // print_heap();

    CU_ASSERT(p1 - 1 == heap);
    CU_ASSERT(*p1 != FREE_BLOCK);

    CU_ASSERT(libre == 10 + 1);
    CU_ASSERT(*(heap + libre) == 116);
    CU_ASSERT(*(heap + libre + 1) == FREE_BLOCK);

    char *p2 = tas_malloc(9);
    // show_tas();
    // print_heap();

    libre = *get_first_libre();
    CU_ASSERT(p2 == heap + 12);
    CU_ASSERT(*(p2 - 1) == 9);
    CU_ASSERT(libre == 21);

    char *p3 = tas_malloc(5);
    // show_tas();
    // print_heap();

    libre = *get_first_libre();
    CU_ASSERT(p3 == heap + 22);
    CU_ASSERT(*(p3 - 1) == 5);
    CU_ASSERT(libre == 27);

    char *p4 = tas_malloc(101);
    // show_tas();
    // print_heap();

    CU_ASSERT(p4 == NULL);

    reset_heap();
}

void test_heap_free_several()
{
    char *heap = get_tas();
    int libre;

    char *p1 = tas_malloc(10);
    char *p2 = tas_malloc(10);
    char *p3 = tas_malloc(10);
    char *p4 = tas_malloc(10);

    strcpy(p1, "tp1");
    strcpy(p2, "tp2");
    strcpy(p3, "tp3");
    strcpy(p4, "tp4");

    tas_free(p2); // simple free

    libre = *get_first_libre();
    CU_ASSERT(*(p2 - 1) == 10);
    CU_ASSERT(*(p2) == 44);
    CU_ASSERT(libre == p2 - 1 - heap);

    tas_free(p3); // testing merge left
    print_heap();

    libre = *get_first_libre();
    CU_ASSERT(*(p2 - 1) == 21);
    CU_ASSERT(*(p2) == 44);
    CU_ASSERT(libre == p2 - 1 - heap);

    tas_free(p1); // testing merge right

    libre = *get_first_libre();
    CU_ASSERT(*(p1 - 1) == 32);
    CU_ASSERT(*(p1) == 44);
    CU_ASSERT(libre == 0);

    reset_heap();
}

void test_best_malloc()
{
    t_strategy *strategy;

    strategy = get_strategy();
    *strategy = &best_fit;

    char *p1 = tas_malloc(20);
    char *p2 = tas_malloc(10);
    char *p3 = tas_malloc(20);
    char *p4 = tas_malloc(8);
    char *p5 = tas_malloc(10);

    strcpy(p1, "tp 1");
    strcpy(p2, "tp 2");
    strcpy(p3, "tp 3");
    strcpy(p4, "tp 4");
    strcpy(p5, "tp 5");
    tas_free(p2);
    tas_free(p4);
    p2 = tas_malloc(5);
    strcpy(p2, "tp 2");

    CU_ASSERT(p2 == p4);
    CU_ASSERT(*(p2 - 1) == 5);

    reset_heap();
}

void test_worst_malloc()
{
    t_strategy *strategy;

    strategy = get_strategy();
    *strategy = &worst_fit;

    char *p1 = tas_malloc(20);
    char *p2 = tas_malloc(10);
    char *p3 = tas_malloc(20);
    char *p4 = tas_malloc(8);
    char *p5 = tas_malloc(10);

    strcpy(p1, "tp 1");
    strcpy(p2, "tp 2");
    strcpy(p3, "tp 3");
    strcpy(p4, "tp 4");
    strcpy(p5, "tp 5");
    tas_free(p2);
    tas_free(p4);
    p2 = tas_malloc(5);
    strcpy(p2, "tp 2");

    CU_ASSERT(p2 == p5 + 11);
    CU_ASSERT(*(p2 - 1) == 5);

    reset_heap();
}

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

int main(void)
{
    CU_pSuite pSuite = NULL;

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

    /* add the tests to the suite */
    if (
        NULL == CU_add_test(pSuite, "of test_tas_malloc()", test_tas_malloc) ||
        NULL == CU_add_test(pSuite, "of test_tas_malloc_empty()", test_tas_malloc_empty) ||
        NULL == CU_add_test(pSuite, "of test_heap_malloc_example()", test_heap_malloc_example) ||
        NULL == CU_add_test(pSuite, "of test_heap_free_several()", test_heap_free_several) ||
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