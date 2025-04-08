#ifndef HASH_TABLE_SRC_SMASH_MAP_VERIFICATION_H
#define HASH_TABLE_SRC_SMASH_MAP_VERIFICATION_H

#include <assert.h>

#include "smash_map/structs.h"
#include "smash_map/dumb/dumb.h"

enum SmashMapError
{
    SMASH_MAP_ERROR_SUCCESS                 = 0,
    SMASH_MAP_ERROR_STANDARD_ERRNO          = 1,
    SMASH_MAP_ERROR_FIST                    = 2,
    SMASH_MAP_ERROR_MAP_IS_NULL             = 3,
    SMASH_MAP_ERROR_MAP_IS_INVALID          = 4,
    SMASH_MAP_ERROR_BUCKETS_IS_NULL         = 5,
    SMASH_MAP_ERROR_BUCKETS_IS_INVALID      = 6,
    SMASH_MAP_ERROR_BUCKETS_SIZE_IS_ZERO    = 7,
    SMASH_MAP_ERROR_KEY_SIZE_IS_ZERO        = 8,
    SMASH_MAP_ERROR_VAL_SIZE_IS_ZERO        = 9,
    SMASH_MAP_ERROR_UNKNOWN                 = 32,
};
static_assert(SMASH_MAP_ERROR_SUCCESS == 0);

const char* smash_map_strerror(const enum SmashMapError error);

#define SMASH_MAP_ERROR_HANDLE(call_func, ...)                                                      \
    do {                                                                                            \
        enum SmashMapError error_handler = call_func;                                               \
        if (error_handler)                                                                          \
        {                                                                                           \
            fprintf(stderr, "Can't " #call_func". Error: %s\n",                                     \
                            smash_map_strerror(error_handler));                                     \
            __VA_ARGS__                                                                             \
            return error_handler;                                                                   \
        }                                                                                           \
    } while(0)


#ifndef NDEBUG

enum SmashMapError smash_map_verify_NOT_USE(const smash_map_t* const map, elem_to_str_t elem_to_str);

#define SMASH_MAP_VERIFY(map, elem_to_str)                                                          \
        do {                                                                                        \
            const enum SmashMapError error = smash_map_verify_NOT_USE(map, elem_to_str);            \
            if (error)                                                                              \
            {                                                                                       \
                SMASH_MAP_DUMB(map, elem_to_str);                                                   \
                lassert(false, "Smash_map error: %s", smash_map_strerror(error));                   \
            }                                                                                       \
        } while(0)

#else /*NDEBUG*/

#define SMASH_MAP_VERIFY(map, elem_to_str) do {} while(0)

#endif /*NDEBUG*/

#endif /*HASH_TABLE_SRC_SMASH_MAP_VERIFICATION_H*/