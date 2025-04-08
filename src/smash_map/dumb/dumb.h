#ifndef HASH_TABLE_SRC_SMASH_MAP_DUMB_DUMB_H
#define HASH_TABLE_SRC_SMASH_MAP_DUMB_DUMB_H

#include "smash_map/structs.h"
#include "logger/liblogger.h"
#include "list_on_array/libfist.h"

// enum FistDumbError
// {
//     FIST_DUMB_ERROR_SUCCESS = 0,
//     FIST_DUMB_ERROR_FAILURE = 1
// };
// static_assert(FIST_DUMB_ERROR_SUCCESS == 0);

// const char* fist_dumb_strerror(const enum FistDumbError error);

// #define FIST_DUMB_ERROR_HANDLE(call_func, ...)                                                      \
//     do {                                                                                            \
//         enum FistDumbError error_handler = call_func;                                               \
//         if (error_handler)                                                                          \
//         {                                                                                           \
//             fprintf(stderr, "Can't " #call_func". Error: %s\n",                                     \
//                             fist_dumb_strerror(error_handler));                                     \
//             __VA_ARGS__                                                                             \
//             return error_handler;                                                                   \
//         }                                                                                           \
//     } while(0)

void smash_map_dumb_NOT_USE(const smash_map_t* const map, const place_in_code_t call_place, 
                            elem_to_str_t elem_to_str);

#define SMASH_MAP_DUMB(map, elem_to_str)                                                            \
        smash_map_dumb_NOT_USE(map,                                                                 \
                               (place_in_code_t)                                                    \
                               { .file = __FILE__, .func = __func__, .line = __LINE__ },            \
                               elem_to_str)

#endif /*HASH_TABLE_SRC_SMASH_MAP_DUMB_DUMB_H*/