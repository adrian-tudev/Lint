#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <stdbool.h>
#include <stdlib.h>

#include "utils/value.h"

typedef struct HashMap HashMap;

HashMap* hm_create(void);
void hm_free(HashMap* map);

// Associates the specified value with the specified key in this map.
// If the map previously contained a mapping for the key, the old
// value is replaced by the specified value.
// The HashMap takes ownership of the value.
// Returns true on success.
bool hm_set(HashMap* map, const char* key, Value* value);

// Returns the value to which the specified key is mapped,
// or NULL if this map contains no mapping for the key.
// The returned value is owned by the HashMap. Do not free it.
Value* hm_get(const HashMap* map, const char* key);

// Removes the mapping for a key from this map if it is present.
// Returns true if a value was removed.
bool hm_delete(HashMap* map, const char* key);

// Prints all key-value pairs in the hashmap for debugging.
void hm_print(const HashMap* map);

// Returns a dynamically allocated array of keys.
// The caller is responsible for freeing the array, but NOT the keys themselves.
// The keys remain owned by the HashMap.
// The number of keys is written to out_count.
// Returns NULL if the map is empty or allocation fails.
char** hm_get_keys(const HashMap* map, size_t* out_count);

// Creates a deep copy of the hashmap.
// Returns NULL if memory allocation fails.
HashMap* hm_copy(const HashMap* map);

#endif // HASHMAP_H_
