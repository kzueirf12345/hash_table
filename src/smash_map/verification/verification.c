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
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_FIST_VALS);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_FIST_KEYS);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_MAP_IS_NULL);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_MAP_IS_INVALID);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_UNKNOWN);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_BUCKETS_IS_NULL);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_BUCKETS_IS_INVALID);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_BUCKETS_SIZE_IS_ZERO);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_FIST_ELEM_SIZE_NEQ);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_NFOUND_ELEM);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_HASH_FUNC_IS_NULL);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_HASH_FUNC_IS_INVALID);
        CASE_ENUM_TO_STRING_(SMASH_MAP_ERROR_FOUND_DUPLICATE);
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

    if (map->size == 0) return SMASH_MAP_ERROR_BUCKETS_SIZE_IS_ZERO;
    
    static_assert(FIST_ERROR_SUCCESS == 0, ""); 
    for (size_t bucket_ind = 0; bucket_ind < map->size; ++bucket_ind)
    {
        enum FistError error_keys = fist_verify(&map->buckets[bucket_ind].keys);
        if (error_keys)
        {
            fprintf(stderr, "Fist error: %s\n", fist_strerror(error_keys));
            return SMASH_MAP_ERROR_FIST_KEYS;
        }

        enum FistError error_vals = fist_verify(&map->buckets[bucket_ind].vals);
        if (error_vals)
        {
            fprintf(stderr, "Fist error: %s\n", fist_strerror(error_vals));
            return SMASH_MAP_ERROR_FIST_VALS;
        }
    }

    for (size_t bucket_ind = 0; bucket_ind < map->size; ++bucket_ind)
    {
        const fist_t keys = map->buckets[bucket_ind].keys;

        for (size_t ind = keys.next[0]; ind != 0; ind = keys.next[ind])
        {
            const void* const key = ((const char*)keys.data + ind * keys.elem_size);

            for (size_t bucket_ind_intern = 0; bucket_ind_intern < map->size; ++bucket_ind_intern)
            {
                const fist_t keys_intern = map->buckets[bucket_ind_intern].keys;

                for (size_t ind_intern = keys_intern.next[0]; 
                     ind_intern != 0; 
                     ind_intern = keys_intern.next[ind_intern])
                {
                    const void* const key_intern 
                        = (const char*)keys_intern.data + ind_intern * keys_intern.elem_size;

                    if (key_intern == key || !key_intern || !key )
                    {
                        continue;
                    }

                    // fprintf(stderr, RED_TEXT("key: %s; key_intern: %s\n"), 
                    //             (const char*)key, (const char*)key_intern);

                    if (keys_intern.elem_size == keys.elem_size
                     && memcmp(key, key_intern, keys.elem_size) == 0)
                    {
                        fprintf(stderr, RED_TEXT("key: %s; key_intern: %s\n"), 
                                (const char*)key, (const char*)key_intern);
                                
                        return SMASH_MAP_ERROR_FOUND_DUPLICATE;
                    }
                }
            }
        }
    }

    return SMASH_MAP_ERROR_SUCCESS;
}