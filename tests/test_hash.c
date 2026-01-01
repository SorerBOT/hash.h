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
    int z = 99;
    int p = 888;
    int l = 90;

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

    hash_set(table, "cool_key", &z);
    value = hash_get(table, "cool_key");
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 2);
    CUNIT_ASSERT_PTR_NOT_NULL(value);
    CUNIT_ASSERT_INT_EQ(*value, 99);

    hash_set(table, "boop", &p);
    value = hash_get(table, "boop");
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 3);
    CUNIT_ASSERT_PTR_NOT_NULL(value);
    CUNIT_ASSERT_INT_EQ(*value, 888);

    hash_set(table, "loop", &l);
    value = hash_get(table, "loop");
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 4);
    CUNIT_ASSERT_PTR_NOT_NULL(value);
    CUNIT_ASSERT_INT_EQ(*value, 90);

    const char** all_keys = hash_get_all_keys(table);
    for (size_t i = 0; i < table->current_occupancy; ++i)
    {
        printf("%lu: key: %s\n", i, all_keys[i]);
    }

    const int** all_values = (const int**) hash_get_all_values(table);
    for (size_t i = 0; i < table->current_occupancy; ++i)
    {
        printf("%lu: value: %d\n", i, *all_values[i]);
    }
}
