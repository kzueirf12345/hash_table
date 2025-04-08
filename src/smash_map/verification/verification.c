#include <stdio.h>

#include "logger/liblogger.h"
#include "smash_map/verification/verification.h"

#define CASE_ENUM_TO_STRING_(error) case error: return #error
const char* smash_map_strerror(const enum SmashMapError error)
{
    switch (error)
    {
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_SUCCESS);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_STANDARD_ERRNO);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_FIST);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_MAP_IS_NULL);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_MAP_IS_INVALID);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_UNKNOWN);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_BUCKETS_IS_NULL);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_BUCKETS_IS_INVALID);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_BUCKETS_SIZE_IS_ZERO);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_KEY_SIZE_IS_ZERO);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_VAL_SIZE_IS_ZERO);


        default:
            return "UNKNOWN_SMASH_MAP_ERROR";
    }
    return "UNKNOWN_SMASH_MAP_ERROR";
}
#undef CASE_ENUM_TO_STRING_


enum SmashMapError smash_map_verify_NOT_USE(const smash_map_t* const map, elem_to_str_t elem_to_str)
{
    switch (is_invalid_ptr(map)) 
    {
        case PTR_STATES_VALID:      break;
        case PTR_STATES_NULL:       return SMASH_MAP_ERROR_MAP_IS_NULL;
        case PTR_STATES_INVALID:    return SMASH_MAP_ERROR_MAP_IS_INVALID;
        case PTR_STATES_ERROR:      return SMASH_MAP_ERROR_STANDARD_ERRNO;

        default:
            fprintf(stderr, "Unknown PtrState enum smash map, it's soooo bad\n");
            return SMASH_MAP_ERROR_UNKNOWN;
    }

    switch (is_invalid_ptr(map->buckets)) 
    {
        case PTR_STATES_VALID:      break;
        case PTR_STATES_NULL:       return SMASH_MAP_ERROR_BUCKETS_IS_NULL;
        case PTR_STATES_INVALID:    return SMASH_MAP_ERROR_BUCKETS_IS_INVALID;
        case PTR_STATES_ERROR:      return SMASH_MAP_ERROR_STANDARD_ERRNO;

        default:
            fprintf(stderr, "Unknown PtrState enum smash map, it's soooo bad\n");
            return SMASH_MAP_ERROR_UNKNOWN;
    }

    if (map->size       == 0) return SMASH_MAP_ERROR_BUCKETS_SIZE_IS_ZERO;
    if (map->key_size   == 0) return SMASH_MAP_ERROR_KEY_SIZE_IS_ZERO;
    if (map->val_size   == 0) return SMASH_MAP_ERROR_VAL_SIZE_IS_ZERO;
    
    for (size_t bucket_ind = 0; bucket_ind < map->size; ++bucket_ind)
    {
        FIST_VERIFY(&map->buckets[bucket_ind], elem_to_str);
    }

    return SMASH_MAP_ERROR_SUCCESS;
}