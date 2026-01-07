#define HASH_IMPLEMENTATION
#include "../src/hash.h"

int main()
{
    int my_favorite_number = 7;
    const int* value = NULL;

    hash_table_t* table = hash_init();
    hash_set(table, "My Favorite Number", &my_favorite_number);
    value = hash_get(table, "My Favorite Number");
    printf("My favorite number is: %d\n", *value);

    return 0;
}
