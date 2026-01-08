#include "hashmap.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "utils/value.h"

#define HASHMAP_MAX_LOAD 0.75

typedef struct Entry {
    char* key;
    Value* value;
    struct Entry* next;
} Entry;

struct HashMap {
    Entry** buckets;
    size_t capacity;
    size_t count;
};

static uint32_t hash_string(const char* key) {
    uint32_t hash = 2166136261u;
    for (size_t i = 0; key[i] != '\0'; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

static Entry* new_entry(const char* key, Value* value) {
    Entry* entry = malloc(sizeof(Entry));
    if (entry == NULL) return NULL;

    size_t key_len = strlen(key);
    entry->key = malloc(key_len + 1);
    if (entry->key == NULL) {
        free(entry);
        return NULL;
    }
    memcpy(entry->key, key, key_len);
    entry->key[key_len] = '\0';
    
    entry->value = value;
    entry->next = NULL;
    return entry;
}

static void free_entry(Entry* entry) {
    if (entry == NULL) return;
    free(entry->key);
    free_value(entry->value);
    free(entry);
}

HashMap* hm_create() {
    HashMap* map = malloc(sizeof(HashMap));
    if (map == NULL) return NULL;
    map->count = 0;
    map->capacity = 8;
    map->buckets = calloc(map->capacity, sizeof(Entry*));
    if (map->buckets == NULL) {
        free(map);
        return NULL;
    }
    return map;
}

void hm_free(HashMap* map) {
    if (map == NULL) return;
    for (size_t i = 0; i < map->capacity; i++) {
        Entry* entry = map->buckets[i];
        while (entry != NULL) {
            Entry* next = entry->next;
            free_entry(entry);
            entry = next;
        }
    }
    free(map->buckets);
    free(map);
}

// Forward declaration for hm_set
static bool hm_adjust_capacity(HashMap* map, size_t capacity);

bool hm_set(HashMap* map, const char* key, Value* value) {
    if (map->count + 1 > map->capacity * HASHMAP_MAX_LOAD) {
        size_t new_capacity = map->capacity < 8 ? 8 : map->capacity * 2;
        if (!hm_adjust_capacity(map, new_capacity)) {
            free_value(value); // Value is owned by caller on failure
            return false;
        }
    }

    uint32_t hash = hash_string(key);
    size_t index = hash % map->capacity;
    
    Entry* entry = map->buckets[index];
    while (entry != NULL) {
        if (strcmp(key, entry->key) == 0) {
            // Key found, replace value
            free_value(entry->value);
            entry->value = value;
            return true;
        }
        entry = entry->next;
    }
    
    // Key not found, walk to end of list and append
    entry = map->buckets[index];
    if (entry == NULL) {
        map->buckets[index] = new_entry(key, value);
        if(map->buckets[index] == NULL) return false;
    } else {
        while(entry->next != NULL) {
            entry = entry->next;
        }
        entry->next = new_entry(key, value);
        if(entry->next == NULL) return false;
    }

    map->count++;
    return true;
}

static bool hm_adjust_capacity(HashMap* map, size_t capacity) {
    Entry** new_buckets = calloc(capacity, sizeof(Entry*));
    if (new_buckets == NULL) return false;

    for (size_t i = 0; i < map->capacity; i++) {
        Entry* entry = map->buckets[i];
        while (entry != NULL) {
            Entry* next = entry->next;
            uint32_t hash = hash_string(entry->key);
            size_t index = hash % capacity;
            
            entry->next = new_buckets[index];
            new_buckets[index] = entry;

            entry = next;
        }
    }

    free(map->buckets);
    map->buckets = new_buckets;
    map->capacity = capacity;
    return true;
}

Value* hm_get(const HashMap* map, const char* key) {
    if (map == NULL || map->count == 0) return NULL;
    
    uint32_t hash = hash_string(key);
    size_t index = hash % map->capacity;
    
    Entry* entry = map->buckets[index];
    while (entry != NULL) {
        if (strcmp(key, entry->key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    
    return NULL;
}

bool hm_delete(HashMap* map, const char* key) {
    if (map == NULL || map->count == 0) return false;
    
    uint32_t hash = hash_string(key);
    size_t index = hash % map->capacity;
    
    Entry* entry = map->buckets[index];
    Entry* prev = NULL;

    while (entry != NULL) {
        if (strcmp(key, entry->key) == 0) {
            if (prev == NULL) {
                map->buckets[index] = entry->next;
            } else {
                prev->next = entry->next;
            }
            free_entry(entry);
            map->count--;
            return true;
        }
        prev = entry;
        entry = entry->next;
    }
    
    return false;
}
