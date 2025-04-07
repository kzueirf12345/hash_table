#ifndef HASH_TABLE_SRC_SMASH_MAP_FUNCS_H
#define HASH_TABLE_SRC_SMASH_MAP_FUNCS_H

#include "smash_map/verification/verification.h"
#include "smash_map/structs.h"

enum SmashMapError smash_map_ctor(smash_map_t* const map, const size_t size, 
                                  const size_t key_size, const size_t val_size);
void               smash_map_dtor(smash_map_t* const map);

#endif /*HASH_TABLE_SRC_SMASH_MAP_FUNCS_H*/