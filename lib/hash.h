#ifndef HASH_H
#define HASH_H

#define HASH_INITIAL_SIZE 16

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _hash_linked_list_t
{
    struct _hash_linked_list_t* next_node;
    const char* key;
    void* value;
} hash_linked_list_t;

typedef struct
{
    size_t size;
    size_t current_occupancy;
    hash_linked_list_t* data;
} hash_table_t;

hash_table_t* hash_init();
int hash_key(const char* key);
void hash_insert(hash_table_t* table, const char* key, void* value);

#endif /* HASH_H */

#ifdef HASH_IMPLEMENTATION

hash_table_t* hash_init()
{
    hash_linked_list_t* data = malloc(HASH_INITIAL_SIZE * sizeof(hash_linked_list_t));
    if (data == NULL)
    {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    hash_table_t* table = malloc(sizeof(hash_table_t));

    if (table == NULL)
    {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    *table = (hash_table_t)
    {
        .current_occupancy = 0,
        .size = HASH_INITIAL_SIZE,
        .data = data
    };

    return table;
}

int hash_key(const char* key)
{

}

void hash_insert(hash_table_t* table, const char* key, void* value)
{
    
}

#endif /* HASH_IMPLEMENTATION */
