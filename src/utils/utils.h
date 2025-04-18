#ifndef HASH_TABLE_SRC_UTILS_UTILS_H
#define HASH_TABLE_SRC_UTILS_UTILS_H

#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <SDL2/SDL.h>

#include "concole.h"

#define HASH_KEY 31ul

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#ifndef NDEBUG

#define IF_DEBUG(...) __VA_ARGS__
#define IF_ELSE_DEBUG(smth, other_smth) smth

#define IF_NDEBUG(...)
#define IF_ELSE_NDEBUG(smth, other_smth) other_smth

#else /*NDEBUG*/

#define IF_DEBUG(...)
#define IF_ELSE_DEBUG(smth, other_smth) other_smth

#define IF_NDEBUG(...) __VA_ARGS__
#define IF_ELSE_NDEBUG(smth, other_smth) smth

#endif /*NDEBUG*/


#define INT_ERROR_HANDLE(call_func, ...)                                                            \
    do {                                                                                            \
        int error_handler = call_func;                                                              \
        if (error_handler)                                                                          \
        {                                                                                           \
            fprintf(stderr, "Can't " #call_func". Errno: %d\n",                                     \
                            errno);                                                                 \
            __VA_ARGS__                                                                             \
            return error_handler;                                                                   \
        }                                                                                           \
    } while(0)

enum PtrState
{
    PTR_STATES_VALID   = 0,
    PTR_STATES_NULL    = 1,
    PTR_STATES_INVALID = 2,
    PTR_STATES_ERROR   = 3
};
static_assert(PTR_STATES_VALID == 0, "");

enum PtrState is_invalid_ptr(const void* ptr);

int is_empty_file (FILE* file);

#endif /*HASH_TABLE_SRC_UTILS_UTILS_H*/