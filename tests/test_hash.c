#define CUNIT_IMPLEMENTATION
#include "external/cunit.h"

#define HASH_IMPLEMENTATION
#include "../lib/hash.h"

CUNIT_TEST(hash)
{
    hash_table_t* table = hash_init();
    CUNIT_ASSERT_PTR_NOT_NULL(table);
    int x = 5;
    hash_set(table, "sha256", &x);
    int* value = hash_get(table, "sha256");
    CUNIT_ASSERT_PTR_NOT_NULL(value);
    CUNIT_ASSERT_INT_EQ(*value, 5);
}
