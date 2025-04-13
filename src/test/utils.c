#include <stdio.h>
#include <stdlib.h>

#include "test/utils.h"
#include "utils/utils.h"
#include "logger/liblogger.h"

size_t string_hash_func(const void* const string)
{
    lassert(!is_invalid_ptr(string), "");

    size_t hash_res = 0;

    asm (
        "crc32q (%1), %0\n\t"               // CRC64 first  64bit
        "crc32q 8(%1), %0\n\t"              // CRC64 second 64bit
        "crc32q 16(%1), %0\n\t"             // CRC64 third  64bit
        "crc32q 24(%1), %0\n\t"             // CRC64 fourth 64bit
        : "+r"(hash_res)
        : "r" (string)
        : "memory"
    );

    return hash_res;
}

int smash_map_key_to_str   (const void* const elem, const size_t   elem_size,
                            char* const *     str,  const size_t mx_str_size)
{
    if (is_invalid_ptr(str))  return -1;
    if (is_invalid_ptr(*str)) return -1;
    (void)elem_size;

    if (elem && *(const char*)elem)
    {
        if (snprintf(*str, mx_str_size, "'%s'", (const char*)elem) <= 0)
        {
            perror("Can't snprintf key to str");
            return -1;
        }
    }
    else
    {
        if (snprintf(*str, mx_str_size, "(nul)") < 0)
        {
            perror("Can't snprintf key (nul) to str");
            return -1;
        }
    }

    return 0;
}
int smash_map_val_to_str   (const void* const elem, const size_t   elem_size,
                            char* const *     str,  const size_t mx_str_size)
{
    if (is_invalid_ptr(str))  return -1;
    if (is_invalid_ptr(*str)) return -1;
    (void)elem_size;

    if (elem)
    {
        if (snprintf(*str, mx_str_size, "'%zu'", *(const size_t*)elem) <= 0)
        {
            perror("Can't snprintf val to str");
            return -1;
        }
    
    }
    else
    {
        if (snprintf(*str, mx_str_size, "(nul)") < 0)
        {
            perror("Can't snprintf key (nul) to str");
            return -1;
        }
    }

    return 0;
}