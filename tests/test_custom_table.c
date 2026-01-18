#include <stdlib.h>

#include "external/cunit.h"
#include "../src/hash.h"

#define ITEMS_COUNT 1000

typedef struct
{
    const char name[32];
    const char id[10];
    size_t age;
} Person;

/* Wrapper over the hash_func_string provided by the library
 * effectively demonstrating how most structs can be hashed by
 * simply turning them into a byte-array
 */
size_t hash_func_person(const void* _person)
{
    const Person* person = _person;
    const char* person_bytes = (const char*)person;
    char person_str[sizeof(Person) + 1];
    strncpy(person_str, person_bytes, sizeof(Person));
    person_str[sizeof(Person)] = '\0';
    return hash_func_string(person_str);
}

CUNIT_TEST(hash_expansion)
{
    hash_table_t* table = hash_init(hash_func_person);
    CUNIT_ASSERT_PTR_NOT_NULL(table);
    CUNIT_ASSERT_INT_EQ(table->size, HASH_INITIAL_SIZE);
    CUNIT_ASSERT_INT_EQ(table->current_occupancy, 0);

    Person herzl =
    {
        .name = "Theodor Herzl",
        .id = "1",
        .age = 166,
    };

    Person david =
    {
        .name = "King David",
        .id = "2",
        .age = 986,
    };

    hash_set(table, &herzl, "was an Austro-Hungarian Jewish journalist and lawyer who was the father of modern political Zionism.");
    hash_set(table, &david, "was a king of ancient Israel and Judah,[6][7] according to the Hebrew Bible and Old Testament.");


    const char* herzl_wikipedia = hash_get(table, &herzl);
    CUNIT_ASSERT_PTR_NOT_NULL(herzl_wikipedia);
    CUNIT_ASSERT_STR_EQ(herzl_wikipedia, "was an Austro-Hungarian Jewish journalist and lawyer who was the father of modern political Zionism.");



    const char* david_wikipedia = hash_get(table, &david);
    CUNIT_ASSERT_PTR_NOT_NULL(david_wikipedia);
    CUNIT_ASSERT_STR_EQ(david_wikipedia, "was a king of ancient Israel and Judah,[6][7] according to the Hebrew Bible and Old Testament.");

    hash_free(table);
}
