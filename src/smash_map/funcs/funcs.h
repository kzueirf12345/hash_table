#ifndef HASH_TABLE_SRC_SMASH_MAP_FUNCS_H
#define HASH_TABLE_SRC_SMASH_MAP_FUNCS_H

#include "smash_map/verification/verification.h"
#include "smash_map/structs.h"

enum SmashMapError smash_map_ctor_NOT_USE(smash_map_t* const map, const size_t size, 
                                          const size_t key_size, const size_t val_size,
                                          hash_func_t hash_func,
                                          const char* const name, const place_in_code_t burn_place);

#ifndef NDEBUG

#define SMASH_MAP_CTOR(map, size, key_size, val_size, hash_func)                                    \
    smash_map_ctor_NOT_USE(map, size, key_size, val_size, hash_func, #map,                          \
                           (place_in_code_t)                                                        \
                           {.file = __FILE__, .func = __func__, .line = __LINE__})

#else /*NDEBUG*/

#define SMASH_MAP_CTOR(map, size, key_size, val_size, hash_func)                                    \
    smash_map_ctor_NOT_USE(map, size, key_size, val_size, NULL, {})

#endif /*NDEBUG*/

void smash_map_dtor(smash_map_t* const map);

#endif /*HASH_TABLE_SRC_SMASH_MAP_FUNCS_H*/