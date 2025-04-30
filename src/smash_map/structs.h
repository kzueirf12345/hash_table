#ifndef HASH_TABLE_SRC_SMASH_MAP_STRUCTS_H
#define HASH_TABLE_SRC_SMASH_MAP_STRUCTS_H

#include <stdlib.h>

#include "../../libs/list_on_array/libfist.h"

typedef struct SmashMapElem
{
    void* key;
    void* val;
} smash_map_elem_t;

typedef struct Bucket
{
    fist_t keys;
    fist_t vals;
} bucket_t;

typedef size_t (*hash_func_t) (const void* const elem);

typedef struct SmashMap
{
    const char* name;
    place_in_code_t burn_place;
    
    bucket_t* buckets;
    size_t size;

    hash_func_t hash_func;
    elem_to_str_t key_to_str;
    elem_to_str_t val_to_str;
    
} smash_map_t;



#endif /*HASH_TABLE_SRC_SMASH_MAP_STRUCTS_H*/