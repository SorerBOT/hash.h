#include "external/cunit.h"
#define GCY_MODE 1
#include "external/gcy.h"

#include "../src/hash.h"

CUNIT_TEST(hash)
{
    hash_table_t* table = hash_init(NULL);
    CUNIT_ASSERT_PTR_NOT_NULL(table);
    CUNIT_ASSERT_INT_EQ(table->size, HASH_INITIAL_SIZE);
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 0);


    char* keys[] =
    {
        "sha256",
        "coolkey",
        "boop",
        "loop"
    };

    int values[] =
    {
        5,
        19,
        99,
        888,
        90
    };

    int final_values[] =
    {
        19,
        99,
        888,
        90
    };

    int x = values[0];
    int y = values[1];
    int z = values[2];
    int p = values[3];
    int l = values[4];

    hash_set(table, keys[0], &x);
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 1);
    const int* value = hash_get(table, keys[0]);
    CUNIT_ASSERT_PTR_NOT_NULL(value);
    CUNIT_ASSERT_INT_EQ(*value, values[0]);

    hash_set(table, keys[0], &y);
    value = hash_get(table, keys[0]);
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 1);
    CUNIT_ASSERT_PTR_NOT_NULL(value);
    CUNIT_ASSERT_INT_EQ(*value, values[1]);

    hash_set(table, keys[1], &z);
    value = hash_get(table, keys[1]);
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 2);
    CUNIT_ASSERT_PTR_NOT_NULL(value);
    CUNIT_ASSERT_INT_EQ(*value, values[2]);

    hash_set(table, keys[2], &p);
    value = hash_get(table, keys[2]);
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 3);
    CUNIT_ASSERT_PTR_NOT_NULL(value);
    CUNIT_ASSERT_INT_EQ(*value, values[3]);

    hash_set(table, keys[3], &l);
    value = hash_get(table, keys[3]);
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 4);
    CUNIT_ASSERT_PTR_NOT_NULL(value);
    CUNIT_ASSERT_INT_EQ(*value, values[4]);

    const char** all_keys = hash_get_all_keys(table);
    CUNIT_ASSERT_ARRAY_STRINGS_IS_PERMUTATION(keys, all_keys, 4);
    GCY_FREE(all_keys);

    const int** all_values = (const int**) hash_get_all_values(table);
    int* all_values_dereferenced = malloc(table->current_occupancy * sizeof(int));
    for (size_t i = 0; i < table->current_occupancy; ++i)
    {
        all_values_dereferenced[i] = *all_values[i];
    }
    GCY_FREE(all_values);
    CUNIT_ASSERT_ARRAY_IS_PERMUTATION(all_values_dereferenced, final_values, sizeof(int), 4);

    hash_free(table);
}
