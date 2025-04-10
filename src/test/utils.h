#ifndef HASH_TABLE_SRC_TEST_UTILS_H
#define HASH_TABLE_SRC_TEST_UTILS_H

#include <stdio.h>
#include <stdlib.h>

#ifndef MAX_WORD_SIZE
#define MAX_WORD_SIZE 64
#endif /*MAX_WORD_SIZE*/

#ifndef MAP_SIZE
#define MAP_SIZE 10007
#endif /*MAP_SIZE*/

size_t string_hash_func     (const void* const string);
int smash_map_key_to_str    (const void* const elem, const size_t   elem_size,
                             char* const *     str,  const size_t mx_str_size);
int smash_map_val_to_str    (const void* const elem, const size_t   elem_size,
                             char* const *     str,  const size_t mx_str_size);

#endif /*HASH_TABLE_SRC_TEST_UTILS_H*/