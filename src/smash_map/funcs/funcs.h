#ifndef HASH_TABLE_SRC_SMASH_MAP_FUNCS_H
#define HASH_TABLE_SRC_SMASH_MAP_FUNCS_H

#include "smash_map/structs.h"
#include "list_on_array/libfist.h"

enum SmashMapError smash_map_ctor_NOT_USE(smash_map_t* const map, const size_t size,
                                          const size_t key_size, const size_t val_size,
                                          hash_func_t hash_func, 
                                          elem_to_str_t key_to_str, elem_to_str_t val_to_str,
                                          const char* const name, const place_in_code_t burn_place);

#ifndef NDEBUG

#define SMASH_MAP_CTOR(map, size, key_size, val_size, hash_func, key_to_str, val_to_str)            \
    smash_map_ctor_NOT_USE(map, size, key_size, val_size, hash_func, key_to_str, val_to_str, #map,  \
                           (place_in_code_t)                                                        \
                           {.file = __FILE__, .func = __func__, .line = __LINE__})

#else /*NDEBUG*/

#define SMASH_MAP_CTOR(map, size, key_size, val_size, hash_func, key_to_str, val_to_str)            \
    smash_map_ctor_NOT_USE(map, size, key_size, val_size, hash_func, key_to_str, val_to_str, NULL,  \
                           (place_in_code_t){})

#endif /*NDEBUG*/

void smash_map_dtor(smash_map_t* const map);

enum SmashMapError smash_map_insert(smash_map_t* const map, const smash_map_elem_t elem);

#endif /*HASH_TABLE_SRC_SMASH_MAP_FUNCS_H*/