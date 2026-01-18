#include <stdlib.h>

#define CUNIT_IMPLEMENTATION
#include "external/cunit.h"

#define GCY_IMPLEMENTATION
#define GCY_MODE 1
#include "external/gcy.h"

#define HASH_MALLOC GCY_MALLOC
#define HASH_CALLOC GCY_CALLOC
#define HASH_FREE GCY_FREE

#define HASH_IMPLEMENTATION
#include "../src/hash.h"

#define ITEMS_COUNT 1000

CUNIT_TEST(hash_memory_leaks)
{
    hash_table_t* table = hash_init();
    CUNIT_ASSERT_PTR_NOT_NULL(table);
    CUNIT_ASSERT_INT_EQ(table->size, HASH_INITIAL_SIZE);
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 0);

    char* keys[ITEMS_COUNT];
    size_t values[ITEMS_COUNT];

    for (size_t i = 0; i < ITEMS_COUNT; ++i)
    {
        values[i] = i + 1;
        keys[i] = malloc(5 * sizeof(char));
        snprintf(keys[i], 5, "%zu", values[i]);
        keys[i][4] = '\0';
        hash_set(table, keys[i], &values[i]);
    }

    for (size_t i = 0; i < ITEMS_COUNT; ++i)
    {
        const size_t* val = hash_get(table, keys[i]);
        CUNIT_ASSERT_PTR_NOT_NULL(val);
        CUNIT_ASSERT_INT_EQ(*val, values[i]);
    }

    hash_free(table);
}
