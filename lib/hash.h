#ifndef HASH_H
#define HASH_H

#define HASH_INITIAL_SIZE 16

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _hash_linked_list_t
{
    struct _hash_linked_list_t* next_node;
    struct _hash_linked_list_t* last_node;
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
size_t hash_key(const char* key);
void hash_set(hash_table_t* table, const char* key, void* value);
void* hash_get(hash_table_t* table, const char* key);

#endif /* HASH_H */

#ifdef HASH_IMPLEMENTATION

static hash_linked_list_t* hash__internal_find_key_in_list(hash_linked_list_t* node_first, const char* key);

static hash_linked_list_t* hash__internal_find_key_in_list(hash_linked_list_t* node_first, const char* key)
{
    hash_linked_list_t* current_node = node_first;
    while (current_node != NULL)
    {
        if ( strcmp(current_node->key, key) == 0 )
        {
            return current_node;
        }
        current_node = current_node->next_node;
    }
    return NULL;
}
hash_table_t* hash_init()
{
    hash_linked_list_t* data = malloc(HASH_INITIAL_SIZE * sizeof(hash_linked_list_t));
    if (data == NULL)
    {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < HASH_INITIAL_SIZE; ++i)
    {
        data->key = NULL;
        data->last_node = NULL;
        data->next_node = NULL;
        data->value = NULL;
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

size_t hash_key(const char* key)
{
    size_t len = strlen(key);
    size_t hash = 0;

    for (size_t i = 0; i < len; ++i)
    {
        hash += (i << 2) * key[i];
    }

    return hash;
}

void hash_set(hash_table_t* table, const char* key, void* value)
{
    size_t hashed_key = hash_key(key);
    hash_linked_list_t* node_first = &table->data[hashed_key % table->size];

    if (node_first->last_node != NULL)
    {
        hash_linked_list_t* entry = hash__internal_find_key_in_list(node_first, key);

        if (entry != NULL)
        {
            entry->value = value;
            return;
        }
    }

    table->current_occupancy++;
    if (node_first->key == NULL)
    {
        *node_first = (hash_linked_list_t)
        {
            .key = key,
            .value = value,
            .last_node = node_first,
            .next_node = NULL
        };
        return;
    }

    hash_linked_list_t* new_node = malloc(sizeof(hash_linked_list_t));
    if (new_node == NULL)
    {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    *new_node = (hash_linked_list_t)
    {
        .key = key,
        .value = value,
        .last_node = new_node,
        .next_node = NULL
    };

    node_first->last_node = new_node;
}
void* hash_get(hash_table_t* table, const char* key)
{
    size_t hashed_key = hash_key(key);
    hash_linked_list_t* node_first = &table->data[hashed_key % table->size];
    hash_linked_list_t* sought_item = hash__internal_find_key_in_list(node_first, key);
    if (sought_item == NULL)
    {
        return NULL;
    }
    return sought_item->value;
}

#endif /* HASH_IMPLEMENTATION */
