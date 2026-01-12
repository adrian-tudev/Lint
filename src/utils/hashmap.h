#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "utils/value.h"
#include <stdbool.h>

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

#endif // HASHMAP_H_
