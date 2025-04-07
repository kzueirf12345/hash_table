#include <stdio.h>

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

        default:
            return "UNKNOWN_SMASH_MAP_ERROR";
    }
    return "UNKNOWN_SMASH_MAP_ERROR";
}
#undef CASE_ENUM_TO_STRING_
