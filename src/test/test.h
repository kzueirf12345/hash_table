#ifndef HASH_TABLE_SRC_TEST_TEST_H
#define HASH_TABLE_SRC_TEST_TEST_H

#include "smash_map/verification/verification.h"

enum SmashMapError print_freq_dict(const char* const input_filename, 
                                   const char* const output_filename);

/*
enum SmashMapError handle_chr_(smash_map_t* const map, const bool is_alpha, const char chr, 
                                      char* const key_buffer, size_t* const key_buffer_counter)
{
    lassert(!is_invalid_ptr(map), "");
    lassert(!is_invalid_ptr(key_buffer), "");

    if (is_alpha)
    {
        IF_DEBUG(
            if (*key_buffer_counter >= MAX_WORD_SIZE)
            {
                fprintf(stderr, "Founded word with size more then max valid\n");
                return SMASH_MAP_ERROR_UNKNOWN;
            }
        )
        key_buffer[(*key_buffer_counter)++] = chr;
    }
    else
    {
        if (!*key_buffer_counter)
        {
            return SMASH_MAP_ERROR_SUCCESS;
        }

        size_t* val = smash_map_get_val(map, key_buffer);
        if (val)
        {
            ++*val;
        }
        else
        {
            size_t temp_val = 1;
            SMASH_MAP_ERROR_HANDLE(
                smash_map_insert(
                    map, 
                    (smash_map_elem_t){.key = key_buffer, .val = &temp_val}
                ),
            );
        }

        *key_buffer_counter = 0;
        _mm256_storeu_si256((__m256i_u*)key_buffer, _mm256_setzero_si256());
    } 

    return SMASH_MAP_ERROR_SUCCESS;
}
*/
#endif /*HASH_TABLE_SRC_TEST_TEST_H*/