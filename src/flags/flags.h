#ifndef DIFFER_SRC_FLAGS_FLAGS_H
#define DIFFER_SRC_FLAGS_FLAGS_H

#include <stdbool.h>

#include "utils/utils.h"

enum FlagsError
{
    FLAGS_ERROR_SUCCESS     = 0,
    FLAGS_ERROR_FAILURE     = 1,
};
static_assert(FLAGS_ERROR_SUCCESS  == 0, "");

const char* flags_strerror(const enum FlagsError error);

#define FLAGS_ERROR_HANDLE(call_func, ...)                                                          \
    do {                                                                                            \
        enum FlagsError error_handler = call_func;                                                  \
        if (error_handler)                                                                          \
        {                                                                                           \
            fprintf(stderr, "Can't " #call_func". Error: %s\n",                                     \
                            flags_strerror(error_handler));                                         \
            __VA_ARGS__                                                                             \
            return error_handler;                                                                   \
        }                                                                                           \
    } while(0)

#ifndef MAX_INOUT_FILES_CNT
#define MAX_INOUT_FILES_CNT 10
#endif /*MAX_INOUT_FILES_CNT*/

typedef struct FlagsObjs
{
    char log_folder         [FILENAME_MAX + 1];
    char input_files        [MAX_INOUT_FILES_CNT][FILENAME_MAX + 1];
    char output_files       [MAX_INOUT_FILES_CNT][FILENAME_MAX + 1];
    size_t cnt_inout_files;

} flags_objs_t;

enum FlagsError flags_objs_ctor (flags_objs_t* const flags_objs);
enum FlagsError flags_objs_dtor (flags_objs_t* const flags_objs);
enum FlagsError flags_processing(flags_objs_t* const flags_objs, 
                                 const int argc, char* const argv[]);



#endif /*DIFFER_SRC_FLAGS_FLAGS_H*/