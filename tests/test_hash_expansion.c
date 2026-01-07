#include <stdlib.h>

#include "external/cunit.h"
#include "../src/hash.h"

#define ITEMS_COUNT 1000

CUNIT_TEST(hash)
{
    hash_table_t* table = hash_init();
    CUNIT_ASSERT_PTR_NOT_NULL(table);
    CUNIT_ASSERT_INT_EQ(table->size, HASH_INITIAL_SIZE);
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 0);

    char** keys = malloc(ITEMS_COUNT * sizeof(char*));
    size_t* values = malloc(ITEMS_COUNT * sizeof(size_t));

    for (size_t i = 0; i < ITEMS_COUNT; ++i)
    {
        values[i] = i + 1;
        keys[i] = malloc(5 * sizeof(char));
        snprintf(keys[i], 5, "%zu", values[i]);
        keys[i][4] = '\0';
        hash_set(table, keys[i], &values[i]);
    }

    CUNIT_ASSERT_INT_EQ(table->size, 512);
}
