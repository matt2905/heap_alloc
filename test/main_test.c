#include "tas_alloc.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

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

    tas_free(p1);
    tas_free(p2);
    tas_free(p3);
    tas_free(p4);
}

void test_tas_malloc_empty(void)
{
    char *p1;

    p1 = (char *)tas_malloc(1);
    strcpy(p1, "\0");
    CU_ASSERT(strcmp(p1, "") == 0);

    tas_free(p1);
}

void test_heap_malloc_example()
{
    char *heap = get_tas();
    char *p1 = tas_malloc(10);
    int libre = *get_first_libre();
    // show_tas();

    CU_ASSERT(p1 - 1 == heap);
    CU_ASSERT(*p1 != FREE_BLOCK);

    CU_ASSERT(libre == 10 + 1);
    CU_ASSERT(*(heap + libre) == 116);
    CU_ASSERT(*(heap + libre + 1) == FREE_BLOCK);

    char *p2 = tas_malloc(9);
    // show_tas();

    libre = *get_first_libre();
    CU_ASSERT(p2 == heap + 12);
    CU_ASSERT(*(p2 - 1) == 9);
    CU_ASSERT(libre == 21);

    char *p3 = tas_malloc(5);
    // show_tas();

    libre = *get_first_libre();
    CU_ASSERT(p3 == heap + 22);
    CU_ASSERT(*(p3 - 1) == 5);
    CU_ASSERT(libre == 27);

    char *p4 = tas_malloc(101);
    // show_tas();

    CU_ASSERT(p4 == NULL);
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
        NULL == CU_add_test(pSuite, "of test_heap_malloc_example()", test_heap_malloc_example))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}