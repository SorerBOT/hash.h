#ifndef HASH_H
#define HASH_H

#define HASH_INITIAL_SIZE 16
#define HASH_EXPANSION_RATE 2
#define HASH_OCCUPANY_RATE 0.7f

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    hash_linked_list_t* data; /* array of linked lists, each linked list contains all colliding keys */
} hash_table_t;

hash_table_t* hash_init();
size_t hash_key(const char* key);
void hash_set(hash_table_t* table, const char* key, void* value);
void* hash_get(hash_table_t* table, const char* key);
const char** hash_get_all_keys(hash_table_t* table);

#endif /* HASH_H */

#ifdef HASH_IMPLEMENTATION

static hash_linked_list_t* hash__internal_find_key_in_list(hash_linked_list_t* node_first, const char* key);
static hash_linked_list_t* hash__internal_find_first_empty_node(hash_linked_list_t* node_first);
static hash_linked_list_t* hash__internal_find_last_node(hash_linked_list_t* node_first);
static hash_table_t* hash__internal_expand_table(hash_table_t* table);

static hash_linked_list_t* hash__internal_find_key_in_list(hash_linked_list_t* node_first, const char* key)
{
    hash_linked_list_t* current_node = node_first;
    while (current_node != NULL)
    {
        if (current_node->key != NULL)
        {
            if ( strcmp(current_node->key, key) == 0 )
            {
                return current_node;
            }
        }
        current_node = current_node->next_node;
    }
    return NULL;
}

static hash_linked_list_t* hash__internal_find_first_empty_node(hash_linked_list_t* node_first)
{
    hash_linked_list_t* current_node = node_first;
    while (current_node != NULL)
    {
        if (current_node->key == NULL)
        {
            return current_node;
        }
        current_node = current_node->next_node;
    }
    return NULL;
}

/* I know {node_first} will never be NULL (by the context of where I use this function).
 * But my general mentality here is to prevent headaches while demonstrating a POC, and not to write spaceship-grade performing code
 */
static hash_linked_list_t* hash__internal_find_last_node(hash_linked_list_t* node_first)
{
    hash_linked_list_t* current_node = node_first;
    if (current_node == NULL)
    {
        return NULL;
    }

    while (current_node->next_node != NULL)
    {
        current_node = current_node->next_node;
    }
    return current_node;
}

//static hash_table_t* hash__internal_expand_table(hash_table_t* table)
//{
//    size_t new_size = table->size * HASH_EXPANSION_RATE;
//    hash_linked_list_t* new_data = malloc(new_size * sizeof(hash_linked_list_t));
//    if ()
//}

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
        data[i].key = NULL;
        data[i].next_node = NULL;
        data[i].value = NULL;
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
    //if (table->current_occupancy / (float)table->size >= HASH_OCCUPANY_RATE)
    //{
    //    hash__internal_expand_table(table);
    //}

    size_t hashed_key = hash_key(key);
    hash_linked_list_t* node_first = &table->data[hashed_key % table->size];

    hash_linked_list_t* entry = hash__internal_find_key_in_list(node_first, key);
    if (entry != NULL)
    {
        entry->value = value;
        return;
    }

    table->current_occupancy++;

    hash_linked_list_t* first_empty_node = hash__internal_find_first_empty_node(node_first);
    if (first_empty_node != NULL)
    {
        *first_empty_node = (hash_linked_list_t)
        {
            .key = key,
            .value = value,
            .next_node = NULL
        };
        return;
    }

    hash_linked_list_t* last_node = hash__internal_find_last_node(node_first);
    if (last_node == NULL)
    {
        fprintf(stderr, "hash table corrupted. item is empty\n");
        exit(EXIT_FAILURE);
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
        .next_node = NULL
    };

    last_node->next_node = new_node;
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

const char** hash_get_all_keys(hash_table_t* table)
{
    const char** all_keys = malloc(table->current_occupancy * sizeof(char*));
    size_t inserted_items_count = 0;

    for (size_t i = 0; i < table->size; ++i)
    {
        hash_linked_list_t* current_node = &table->data[i];
        while (current_node != NULL)
        {
            if (current_node->key != NULL)
            {
                all_keys[inserted_items_count] = current_node->key;
                ++inserted_items_count;
            }
            current_node = current_node->next_node;
        }
    }

    if (inserted_items_count != table->current_occupancy)
    {
        fprintf(stderr, "hash table corrupted. keys are either in excess or are missing.\n");
    }
    return all_keys;
}

#endif /* HASH_IMPLEMENTATION */
