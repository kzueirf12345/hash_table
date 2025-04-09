#ifndef HASH_TABLE_SRC_SMASH_MAP_DUMB_DUMB_H
#define HASH_TABLE_SRC_SMASH_MAP_DUMB_DUMB_H

#include "smash_map/structs.h"
#include "logger/liblogger.h"
#include "list_on_array/libfist.h"


void smash_map_dumb_NOT_USE(const smash_map_t* const map, const place_in_code_t call_place, 
                            elem_to_str_t key_to_str, elem_to_str_t val_to_str);

#define SMASH_MAP_DUMB(map, key_to_str, val_to_str)                                                 \
        smash_map_dumb_NOT_USE(map,                                                                 \
                               (place_in_code_t)                                                    \
                               { .file = __FILE__, .func = __func__, .line = __LINE__ },            \
                               key_to_str, val_to_str)

#endif /*HASH_TABLE_SRC_SMASH_MAP_DUMB_DUMB_H*/