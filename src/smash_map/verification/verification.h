#ifndef HASH_TABLE_SRC_SMASH_MAP_VERIFICATION_H
#define HASH_TABLE_SRC_SMASH_MAP_VERIFICATION_H

#include <assert.h>

enum SmashMapError
{
    SMASH_MAP_ERROR_SUCCESS                 = 0,
    SMASH_MAP_ERROR_STANDARD_ERRNO          = 1,
    SMASH_MAP_ERROR_FIST                    = 2
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

    

#endif /*HASH_TABLE_SRC_SMASH_MAP_VERIFICATION_H*/