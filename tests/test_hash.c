#define CUNIT_IMPLEMENTATION
#include "external/cunit.h"

#define HASH_IMPLEMENTATION
#include "../lib/hash.h"

CUNIT_TEST(hash)
{
    hash_table_t* table = hash_init();
    CUNIT_ASSERT_PTR_NOT_NULL(table);
    CUNIT_ASSERT_INT_EQ(table->size, HASH_INITIAL_SIZE);
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 0);

    int x = 5;
    int y = 19;
    hash_set(table, "sha256", &x);
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 1);
    int* value = hash_get(table, "sha256");
    CUNIT_ASSERT_PTR_NOT_NULL(value);
    CUNIT_ASSERT_INT_EQ(*value, 5);

    hash_set(table, "sha256", &y);
    value = hash_get(table, "sha256");
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 1);
    CUNIT_ASSERT_PTR_NOT_NULL(value);
    CUNIT_ASSERT_INT_EQ(*value, 19);
}
