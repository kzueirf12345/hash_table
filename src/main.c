#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

#include "utils/utils.h"
#include "logger/liblogger.h"
#include "flags/flags.h"
#include "list_on_array/libfist.h"
#include "smash_map/funcs/funcs.h"

size_t string_hash_func         (const string_t string);
size_t string_hash_func_wrapper (const void* string);

int init_all(flags_objs_t* const flags_objs, const int argc, char* const * argv);
int dtor_all(flags_objs_t* const flags_objs);

//==================================================================================================

int main(const int argc, char* const argv[])
{
    flags_objs_t flags_objs  = {};

    INT_ERROR_HANDLE(init_all(&flags_objs, argc, argv));

    smash_map_t map = {};
    SMASH_MAP_ERROR_HANDLE(
        SMASH_MAP_CTOR(&map, 1024, sizeof(char*), sizeof(size_t), string_hash_func_wrapper),
                                                                              dtor_all(&flags_objs);
    );

    SMASH_MAP_VERIFY_ASSERT(&map, NULL);

    smash_map_dtor(&map);

    INT_ERROR_HANDLE(                                                        dtor_all(&flags_objs));

    return EXIT_SUCCESS;
}

//==================================================================================================

size_t string_hash_func(const string_t string)
{
    lassert(!is_invalid_ptr(string.data), "");

    size_t hash_result = 0;
    for (size_t ind = 0; ind < string.size; ++ind)
    {
        hash_result = (size_t)(
            ((HASH_KEY * hash_result) % INT64_MAX + (size_t)string.data[ind]) % INT64_MAX
        );
    }
    return hash_result;
}

size_t string_hash_func_wrapper(const void* string)
{
    lassert(!is_invalid_ptr(string), "");

    return string_hash_func(*(const string_t*)string);
}

int logger_init(char* const log_folder);

int init_all(flags_objs_t* const flags_objs, const int argc, char* const * argv)
{
    lassert(argc, "");
    lassert(!is_invalid_ptr(argv), "");

    if (!setlocale(LC_ALL, "ru_RU.utf8"))
    {
        fprintf(stderr, "Can't setlocale\n");
        return EXIT_FAILURE;
    }

    FLAGS_ERROR_HANDLE(flags_objs_ctor (flags_objs));
    FLAGS_ERROR_HANDLE(flags_processing(flags_objs, argc, argv));

    if (logger_init(flags_objs->log_folder))
    {
        fprintf(stderr, "Can't logger init\n");
                                                                        flags_objs_dtor(flags_objs);
        return EXIT_FAILURE;
    }

    FIST_DUMB_ERROR_HANDLE(fist_dumb_ctor(),
                                                                                      logger_dtor();
                                                                        flags_objs_dtor(flags_objs);
    );

    return EXIT_SUCCESS;
}

int dtor_all(flags_objs_t* const flags_objs)
{
    FIST_DUMB_ERROR_HANDLE(                                                      fist_dumb_dtor(););
    LOGG_ERROR_HANDLE(                                                              logger_dtor(););
    FLAGS_ERROR_HANDLE(                                               flags_objs_dtor(flags_objs););

    return EXIT_SUCCESS;
}

#define LOGOUT_FILENAME "logout.log"
#define   DUMB_FILENAME "dumb"
int logger_init(char* const log_folder)
{
    lassert(!is_invalid_ptr(log_folder), "");

    char logout_filename[FILENAME_MAX] = {};
    if (snprintf(logout_filename, FILENAME_MAX, "%s%s", log_folder, LOGOUT_FILENAME) <= 0)
    {
        perror("Can't snprintf logout_filename");
        return EXIT_FAILURE;
    }

    char dumb_filename[FILENAME_MAX] = {};
    if (snprintf(dumb_filename, FILENAME_MAX, "%s%s", log_folder, DUMB_FILENAME) <= 0)
    {
        perror("Can't snprintf dumb_filename");
        return EXIT_FAILURE;
    }

    LOGG_ERROR_HANDLE(logger_ctor());
    LOGG_ERROR_HANDLE(logger_set_level_details(LOG_LEVEL_DETAILS_ALL));
    LOGG_ERROR_HANDLE(logger_set_logout_file(logout_filename));
    
    return EXIT_SUCCESS;
}
#undef LOGOUT_FILENAME
#undef   DUMB_FILENAME