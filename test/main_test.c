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
        NULL == CU_add_test(pSuite, "of test_tas_malloc_empty()", test_tas_malloc_empty))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}