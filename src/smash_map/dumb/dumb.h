#ifndef HASH_TABLE_SRC_SMASH_MAP_DUMB_DUMB_H
#define HASH_TABLE_SRC_SMASH_MAP_DUMB_DUMB_H

#include "smash_map/structs.h"
#include "logger/liblogger.h"
#include "list_on_array/libfist.h"


void smash_map_dumb_NOT_USE(const smash_map_t* const map, const place_in_code_t call_place);

#define SMASH_MAP_DUMB(map)                                                                         \
        smash_map_dumb_NOT_USE(map,                                                                 \
                               (place_in_code_t)                                                    \
                               { .file = __FILE__, .func = __func__, .line = __LINE__ })

#endif /*HASH_TABLE_SRC_SMASH_MAP_DUMB_DUMB_H*/