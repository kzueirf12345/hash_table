#include <stdio.h>
#include <stdbool.h>

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
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_FIST_ELEM_SIZE_NEQ);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_NFOUND_ELEM);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_HASH_FUNC_IS_NULL);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_HASH_FUNC_IS_INVALID);
        default:
            return "UNKNOWN_SMASH_MAP_ERROR";
    }
    return "UNKNOWN_SMASH_MAP_ERROR";
}
#undef CASE_ENUM_TO_STRING_


enum SmashMapError smash_map_verify(const smash_map_t* const map)
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

    switch (is_invalid_ptr(map->hash_func)) 
    {
        case PTR_STATES_VALID:      break;
        case PTR_STATES_NULL:       return SMASH_MAP_ERROR_HASH_FUNC_IS_NULL;
        case PTR_STATES_INVALID:    return SMASH_MAP_ERROR_HASH_FUNC_IS_INVALID;
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
        enum FistError error = fist_verify(&map->buckets[bucket_ind]);
        static_assert(FIST_ERROR_SUCCESS == 0, ""); 
        if (error)
        {
            fprintf(stderr, "Fist error: %s\n", fist_strerror(error));
            return SMASH_MAP_ERROR_FIST;
        }
    }

    for (size_t bucket_ind = 0; bucket_ind < map->size; ++bucket_ind)
    {
        const fist_t fist = map->buckets[bucket_ind];

        for (size_t ind = fist.next[0]; ind != 0; ind = fist.next[ind])
        {
            const void* const elem = (char*)fist.data + ind * fist.elem_size;
            bool is_find = false;

            for (size_t bucket_ind_intern = 0; bucket_ind_intern < map->size; ++bucket_ind_intern)
            {
                const fist_t fist_intern = map->buckets[bucket_ind_intern];

                if (fist_intern.elem_size != fist.elem_size)
                {
                    return SMASH_MAP_ERROR_FIST_ELEM_SIZE_NEQ;
                }

                const size_t elem_size = fist.elem_size;

                for (size_t ind_intern = fist.next[0]; 
                     ind_intern != 0; 
                     ind_intern = fist.next[ind_intern])
                {
                    const void* const elem_intern 
                        = (char*)fist_intern.data + ind_intern * elem_size;

                    if (memcmp(elem_intern, elem, elem_size) == 0)
                    {
                        is_find = true; 
                    }
                }
            }

            if (!is_find)
            {
                return SMASH_MAP_ERROR_NFOUND_ELEM;
            }
        }
    }

    return SMASH_MAP_ERROR_SUCCESS;
}