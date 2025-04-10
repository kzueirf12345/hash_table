#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#include "flags/flags.h"
#include "logger/liblogger.h"
#include "utils/utils.h"

#define CASE_ENUM_TO_STRING_(error) case error: return #error
const char* flags_strerror(const enum FlagsError error)
{
    switch(error)
    {
        CASE_ENUM_TO_STRING_(FLAGS_ERROR_SUCCESS);
        CASE_ENUM_TO_STRING_(FLAGS_ERROR_FAILURE);
        default:
            return "UNKNOWN_FLAGS_ERROR";
    }
    return "UNKNOWN_FLAGS_ERROR";
}
#undef CASE_ENUM_TO_STRING_


enum FlagsError flags_objs_ctor(flags_objs_t* const flags_objs)
{
    lassert(!is_invalid_ptr(flags_objs), "");

    if (!strncpy(flags_objs->log_folder, "./log/", FILENAME_MAX))
    {
        perror("Can't strncpy flags_objs->log_folder");
        return FLAGS_ERROR_SUCCESS;
    }

    flags_objs->cnt_inout_files = 0;

    return FLAGS_ERROR_SUCCESS;
}

enum FlagsError flags_objs_dtor (flags_objs_t* const flags_objs)
{
    lassert(!is_invalid_ptr(flags_objs), "");

    return FLAGS_ERROR_SUCCESS;
}


enum FlagsError flags_processing(flags_objs_t* const flags_objs, 
                                 const int argc, char* const argv[])
{
    lassert(!is_invalid_ptr(flags_objs), "");
    lassert(!is_invalid_ptr(argv), "");
    lassert(argc, "");

    int getopt_rez = 0;
    while ((getopt_rez = getopt(argc, argv, "l:i:")) != -1)
    {
        switch (getopt_rez)
        {
            case 'l':
            {
                if (!strncpy(flags_objs->log_folder, optarg, FILENAME_MAX))
                {
                    perror("Can't strncpy flags_objs->log_folder");
                    return FLAGS_ERROR_FAILURE;
                }

                break;
            }

            case 'i':
            {
                flags_objs->cnt_inout_files = (size_t)atoll(optarg);

                if (flags_objs->cnt_inout_files     == 0 
                 || flags_objs->cnt_inout_files     >  MAX_INOUT_FILES_CNT)
                {
                    fprintf(stderr, "Error cnt_inout_files value\n");
                    return FLAGS_ERROR_FAILURE;
                }

                if ((size_t)argc - 2ul < flags_objs->cnt_inout_files)
                {
                    fprintf(stderr, "Few arguments in command line\n");
                    return FLAGS_ERROR_FAILURE;
                }

                for (size_t cnt = 0; cnt < flags_objs->cnt_inout_files; ++cnt)
                {
                    if (!strncpy(flags_objs->input_files[cnt], argv[optind++], FILENAME_MAX))
                    {
                        perror("Can't strncpy flags_objs->input_files[i]");
                        return FLAGS_ERROR_FAILURE;
                    }

                    if (!strncpy(flags_objs->output_files[cnt], argv[optind++], FILENAME_MAX))
                    {
                        perror("Can't strncpy flags_objs->output_files[i]");
                        return FLAGS_ERROR_FAILURE;
                    }
                }

                break;
            }

            default:
            {
                fprintf(stderr, "Getopt error - d: %d, c: %c\n", getopt_rez, (char)getopt_rez);
                return FLAGS_ERROR_FAILURE;
            }
        }
    }

    return FLAGS_ERROR_SUCCESS;
}