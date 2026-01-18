/*
 * Copyright (c) 2025 Alon Filler
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef HASH_H
#define HASH_H

#define HASH_INITIAL_SIZE 16
#define HASH_EXPANSION_RATE 2
#define HASH_OCCUPANY_RATE 3

#if defined(HASH_MALLOC) || defined(HASH_CALLOC) || defined(HASH_FREE)
    #ifndef HASH_MALLOC
        #error "If the user seeks to provide malloc(), he must also provide calloc() and free()"
    #endif
    #ifndef HASH_CALLOC
        #error "If the user seeks to provide calloc(), he must also provide malloc() and free()"
    #endif
    #ifndef HASH_FREE
        #error "If the user seeks to provide free(), he must also provide malloc() and calloc()"
    #endif
#else
    #include <stdlib.h>
    #define HASH_MALLOC malloc
    #define HASH_CALLOC calloc
    #define HASH_FREE free
#endif

#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    const char* key;
    const void* value;
} hash_key_value_t;

typedef struct _hash_linked_list_t
{
    struct _hash_linked_list_t* next_node;
    hash_key_value_t key_value;
} hash_linked_list_t;

typedef struct
{
    size_t size;
    size_t current_occupancy;
    hash_linked_list_t** data; /* array of linked lists, each linked list contains all colliding keys */
} hash_table_t;


hash_table_t* hash_init();
size_t hash_key(const char* key);
void hash_set(hash_table_t* table, const char* key, void* value);
const void* hash_get(hash_table_t* table, const char* key);
const char** hash_get_all_keys(hash_table_t* table);
const void** hash_get_all_values(hash_table_t* table);
hash_key_value_t* hash_get_all_key_values(hash_table_t* table);
void hash_free(hash_table_t* table);

#endif /* HASH_H */

#ifdef HASH_IMPLEMENTATION

static hash_linked_list_t* hash__internal_find_key_in_list(hash_linked_list_t* node_first, const char* key);
static hash_linked_list_t* hash__internal_find_first_empty_node(hash_linked_list_t* node_first);
static hash_linked_list_t* hash__internal_find_last_node(hash_linked_list_t* node_first);
static void hash__internal_expand_table(hash_table_t* table);
static void hash__internal_set_in_data(hash_linked_list_t** data, size_t size, const char* key, const void* value, size_t* current_occupancy);
static void hash__internal_free_data(hash_linked_list_t** data, size_t size);

static hash_linked_list_t* hash__internal_find_key_in_list(hash_linked_list_t* node_first, const char* key)
{
    hash_linked_list_t* current_node = node_first;
    while (current_node != NULL)
    {
        if (current_node->key_value.key != NULL)
        {
            if ( strcmp(current_node->key_value.key, key) == 0 )
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
        if (current_node->key_value.key == NULL)
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

static void hash__internal_expand_table(hash_table_t* table)
{
    size_t new_size = table->size * HASH_EXPANSION_RATE;
    hash_linked_list_t** new_data = HASH_CALLOC(new_size, sizeof(hash_linked_list_t*));
    hash_key_value_t* key_values = hash_get_all_key_values(table);

    for (size_t i = 0; i < table->current_occupancy; ++i)
    {
        hash_key_value_t key_value = key_values[i];
        hash__internal_set_in_data(new_data, new_size, key_value.key, key_value.value, NULL);
    }

    HASH_FREE(key_values);
    hash__internal_free_data(table->data, table->size);

    table->data = new_data;
    table->size = new_size;
}

static void hash__internal_set_in_data(hash_linked_list_t** data, size_t size, const char* key, const void* value, size_t* current_occupancy)
{
    size_t hashed_key = hash_key(key);
    hash_linked_list_t* node_first = data[hashed_key % size];

    hash_linked_list_t* entry = hash__internal_find_key_in_list(node_first, key);
    if (entry != NULL)
    {
        entry->key_value.value = value;
        return;
    }

    if (current_occupancy != NULL)
    {
        *current_occupancy += 1;
    }

    hash_linked_list_t* first_empty_node = hash__internal_find_first_empty_node(node_first);
    if (first_empty_node != NULL)
    {
        *first_empty_node = (hash_linked_list_t)
        {
            .key_value = (hash_key_value_t)
            {
                .key = key,
                .value = value
            },
            .next_node = NULL
        };
        return;
    }

    hash_linked_list_t* new_node = HASH_MALLOC(sizeof(hash_linked_list_t));
    if (new_node == NULL)
    {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    *new_node = (hash_linked_list_t)
    {
        .next_node = NULL,
        .key_value = (hash_key_value_t)
        {
            .key = key,
            .value = value
        }
    };

    hash_linked_list_t* last_node = hash__internal_find_last_node(node_first);
    if (last_node == NULL)
    {
        data[hashed_key % size] = new_node;
    }
    else
    {
        last_node->next_node = new_node;
    }
}

hash_table_t* hash_init()
{
    hash_linked_list_t** data = HASH_CALLOC(HASH_INITIAL_SIZE, sizeof(hash_linked_list_t*));
    if (data == NULL)
    {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    hash_table_t* table = HASH_MALLOC(sizeof(hash_table_t));

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
#ifdef HASH_DEBUG
    printf("Setting %s = ???\n. Current occupancy: %lu, table size: %lu, current_occupancy + 1 / size = %f\n", key, table->current_occupancy, table->size, (table->current_occupancy + 1.f) / (float)table->size);
#endif

    if ((table->current_occupancy + 1.f) / (float)table->size >= HASH_OCCUPANY_RATE)
    {
        hash__internal_expand_table(table);
    }
    hash__internal_set_in_data(table->data, table->size, key, value, &table->current_occupancy);
}

const void* hash_get(hash_table_t* table, const char* key)
{
    size_t hashed_key = hash_key(key);
    hash_linked_list_t* node_first = table->data[hashed_key % table->size];
    hash_linked_list_t* sought_item = hash__internal_find_key_in_list(node_first, key);
    if (sought_item == NULL)
    {
        return NULL;
    }
    return sought_item->key_value.value;
}

const char** hash_get_all_keys(hash_table_t* table)
{
    const char** all_keys = HASH_MALLOC(table->current_occupancy * sizeof(char*));
    size_t inserted_items_count = 0;

    for (size_t i = 0; i < table->size; ++i)
    {
        hash_linked_list_t* current_node = table->data[i];
        while (current_node != NULL)
        {
            if (current_node->key_value.key != NULL)
            {
                all_keys[inserted_items_count] = current_node->key_value.key;
                ++inserted_items_count;
            }
            current_node = current_node->next_node;
        }
    }

    if (inserted_items_count != table->current_occupancy)
    {
        fprintf(stderr, "hash table corrupted. Found %lu items out of supposed %lu items.\n", inserted_items_count, table->current_occupancy);
        exit(EXIT_FAILURE);
    }
    return all_keys;
}

const void** hash_get_all_values(hash_table_t* table)
{
    const void** all_values = HASH_MALLOC(table->current_occupancy * sizeof(void*));

    size_t inserted_items_count = 0;

    for (size_t i = 0; i < table->size; ++i)
    {
        hash_linked_list_t* current_node = table->data[i];
        while (current_node != NULL)
        {
            if (current_node->key_value.value != NULL)
            {
                all_values[inserted_items_count] = current_node->key_value.value;
                ++inserted_items_count;
            }
            current_node = current_node->next_node;
        }
    }

    if (inserted_items_count != table->current_occupancy)
    {
        fprintf(stderr, "hash table corrupted. Found %lu items out of supposed %lu items.\n", inserted_items_count, table->current_occupancy);
        exit(EXIT_FAILURE);
    }

    return all_values;
}

hash_key_value_t* hash_get_all_key_values(hash_table_t* table)
{
    hash_key_value_t* all_key_values = HASH_MALLOC(table->current_occupancy * sizeof(hash_key_value_t));

    size_t inserted_items_count = 0;

    for (size_t i = 0; i < table->size; ++i)
    {
        hash_linked_list_t* current_node = table->data[i];
        while (current_node != NULL)
        {
            if (current_node->key_value.key != NULL)
            {
                all_key_values[inserted_items_count] = current_node->key_value;
                ++inserted_items_count;
            }
            current_node = current_node->next_node;
        }
    }

    if (inserted_items_count != table->current_occupancy)
    {
        fprintf(stderr, "hash table corrupted. Found %lu items out of supposed %lu items.\n", inserted_items_count, table->current_occupancy);
        exit(EXIT_FAILURE);
    }

    return all_key_values;
}

static void hash__internal_free_data(hash_linked_list_t** data, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        hash_linked_list_t* current_node = data[i];
        while (current_node != NULL)
        {
            hash_linked_list_t* tmp = current_node;
            current_node = current_node->next_node;
            HASH_FREE(tmp);
        }
    }
    HASH_FREE(data);
}

void hash_free(hash_table_t* table)
{
    hash__internal_free_data(table->data, table->size);
    HASH_FREE(table);
}

#endif /* HASH_IMPLEMENTATION */
