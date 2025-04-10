#ifndef HASH_TABLE_SRC_SMASH_MAP_VERIFICATION_H
#define HASH_TABLE_SRC_SMASH_MAP_VERIFICATION_H

#include <assert.h>

#include "smash_map/structs.h"
#include "smash_map/dumb/dumb.h"

enum SmashMapError
{
    SMASH_MAP_ERROR_SUCCESS                 = 0,
    SMASH_MAP_ERROR_STANDARD_ERRNO          = 1,
    SMASH_MAP_ERROR_FIST_KEYS               = 2,
    SMASH_MAP_ERROR_MAP_IS_NULL             = 3,
    SMASH_MAP_ERROR_MAP_IS_INVALID          = 4,
    SMASH_MAP_ERROR_BUCKETS_IS_NULL         = 5,
    SMASH_MAP_ERROR_BUCKETS_IS_INVALID      = 6,
    SMASH_MAP_ERROR_BUCKETS_SIZE_IS_ZERO    = 7,
    SMASH_MAP_ERROR_FIST_VALS               = 8,
    SMASH_MAP_ERROR_FIST_ELEM_SIZE_NEQ      = 10,
    SMASH_MAP_ERROR_NFOUND_ELEM             = 11,
    SMASH_MAP_ERROR_HASH_FUNC_IS_NULL       = 12,
    SMASH_MAP_ERROR_HASH_FUNC_IS_INVALID    = 13,
    SMASH_MAP_ERROR_FOUND_DUPLICATE         = 14,
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


enum SmashMapError smash_map_verify(const smash_map_t* const map);

#ifndef NDEBUG

#define SMASH_MAP_VERIFY_ASSERT(map)                                                                \
        do {                                                                                        \
            const enum SmashMapError error = smash_map_verify(map);                                 \
            if (error)                                                                              \
            {                                                                                       \
                SMASH_MAP_DUMB(map);                                                                \
                lassert(false, "Smash_map error: %s", smash_map_strerror(error));                   \
            }                                                                                       \
        } while(0)

#else /*NDEBUG*/

#define SMASH_MAP_VERIFY_ASSERT(map) do {} while(0)

#endif /*NDEBUG*/

#endif /*HASH_TABLE_SRC_SMASH_MAP_VERIFICATION_H*/