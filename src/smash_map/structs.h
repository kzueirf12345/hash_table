#ifndef HASH_TABLE_SRC_SMASH_MAP_STRUCTS_H
#define HASH_TABLE_SRC_SMASH_MAP_STRUCTS_H

#include <stdlib.h>

#include "list_on_array/libfist.h"

typedef struct SmashMapElem
{
    void* key;
    void* val;
} smash_map_elem_t;

typedef struct SmashMap
{
    IF_DEBUG(const char* name;)
    IF_DEBUG(place_in_code_t burn_place;)
    
    fist_t* buckets;
    size_t size;

    size_t key_size;
    size_t val_size;
} smash_map_t;



#endif /*HASH_TABLE_SRC_SMASH_MAP_STRUCTS_H*/