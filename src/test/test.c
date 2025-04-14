#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <valgrind/callgrind.h>
#include <valgrind/valgrind.h>
#include <immintrin.h>
#include <stdbool.h>

#include "test/test.h"
#include "utils/utils.h"
#include "logger/liblogger.h"
#include "smash_map/funcs/funcs.h"
#include "smash_map/verification/verification.h"
#include "test/utils.h"

#define MM256_SHL(src, n, dst) \
    do { \
        __m256i mask = _mm256_setzero_si256(); \
        for (size_t i = 0; i < 32 - n; ++i) { ((char*)&mask)[i] = 0xFF; } \
        dst = _mm256_and_si256(src, mask); \
    } while (0)
    
#define MM256_SHR(src, n, dst) \
    do { \
        __m256i mask = _mm256_setzero_si256(); \
        for (size_t i = 0; i < 32 - n; ++i) { ((char*)&mask)[31 - i] = 0xFF; } \
        dst = _mm256_and_si256(src, mask); \
    } while (0)
    

char* get_text_from_file_(const char* const filename, size_t* const size);

enum SmashMapError handle_key_buffer_(smash_map_t* const map, __m256i* const key_buffer);

enum SmashMapError print_freq_dict(const char* const input_filename, 
                                   const char* const output_filename)
{
    lassert(!is_invalid_ptr(input_filename), "");
    lassert(!is_invalid_ptr(output_filename), "");

    size_t text_size = 0;
    char* const text = get_text_from_file_(input_filename, &text_size);
    if (!text)
    {
        fprintf(stderr, "Can't get_text_from_file\n");
        return SMASH_MAP_ERROR_STANDARD_ERRNO;
    }

    smash_map_t map;
    SMASH_MAP_ERROR_HANDLE(
        SMASH_MAP_CTOR(
            &map, 
            MAP_SIZE,
            MAX_WORD_SIZE, 
            sizeof(size_t), 
            string_hash_func, 
            smash_map_key_to_str, 
            smash_map_val_to_str
        ),
        munmap(text, text_size);
    );

    CALLGRIND_START_INSTRUMENTATION;

    const size_t chank_cnt = text_size / MAX_WORD_SIZE;
    const size_t chank_rem = text_size % MAX_WORD_SIZE;

    __m256i key_buffer = _mm256_setzero_si256();
    size_t key_buffer_counter = 0;

    for (size_t chank_ind = 0; chank_ind < chank_cnt; ++chank_ind)
    {
        const __m256i chunk = _mm256_load_si256((const __m256i*)text + chank_ind);

        const __m256i chunk_upper = _mm256_and_si256(chunk, _mm256_set1_epi8((char)0b11011111));

        const __m256i is_alpha = _mm256_and_si256(
            _mm256_cmpgt_epi8(_mm256_set1_epi8('Z' + 1),  chunk_upper), 
            _mm256_cmpgt_epi8(chunk_upper, _mm256_set1_epi8('A' - 1))
        );

        const uint32_t mask = (uint32_t)_mm256_movemask_epi8(is_alpha);

        const uint32_t starts = mask & ((~mask >> 1u) + (1u << (MAX_WORD_SIZE - 1u)));
        const uint32_t ends   = mask & ((~mask << 1u) + 1u);
        
        uint32_t current_pos = 0;
    
        while (current_pos < 32) 
        {
            const uint32_t word_start = _tzcnt_u32(starts >> current_pos) + current_pos;

            if ((ends >> word_start) == 0)
            {
                MM256_SHL(key_buffer, chunk, word_start);
                __m256i start_mask = _mm256_setzero_si256();
                for (size_t i = 0; i < word_start; ++i)
                {
                    ((char*)&start_mask)[i] = 0xFF;
                }
                key_buffer = _mm256_and_si256(chunk, start_mask);
                key_buffer_counter = MAX_WORD_SIZE - word_start;
                break;
            }

            const uint32_t word_end = _tzcnt_u32(ends >> word_start) + word_start;
            const uint32_t word_size = word_end - word_start + 1;
            
            __m256i shifted_chunk = _mm256_srli_si256(chunk, MAX_WORD_SIZE - word_end - 1);

            if (key_buffer_counter > 0)
            {
                shifted_chunk = _mm256_slli_si256(shifted_chunk, MAX_WORD_SIZE - word_size - key_buffer_counter);
                key_buffer = _mm256_or_si256(key_buffer, shifted_chunk);
            } 
            else 
            {
                key_buffer = _mm256_slli_si256(shifted_chunk, MAX_WORD_SIZE - word_size);
            }

            SMASH_MAP_ERROR_HANDLE(handle_key_buffer_(&map, &key_buffer),
                smash_map_dtor(&map);
                munmap(text, text_size);
            );

            key_buffer_counter = 0;
            key_buffer = _mm256_setzero_si256();

            current_pos = word_end + 1;
        }
    }

    __m256i chunk = _mm256_load_si256((const __m256i*)text + chank_cnt);
    chunk = _mm256_srli_si256(chunk, MAX_WORD_SIZE - chank_rem);
    chunk = _mm256_slli_si256(chunk, MAX_WORD_SIZE - chank_rem);

    const __m256i chunk_upper = _mm256_and_si256(chunk, _mm256_set1_epi8((char)0b11011111));

    const __m256i is_alpha = _mm256_and_si256(
        _mm256_cmpgt_epi8(_mm256_set1_epi8('Z' + 1),  chunk_upper), 
        _mm256_cmpgt_epi8(chunk_upper, _mm256_set1_epi8('A' - 1))
    );

    const uint32_t mask = (uint32_t)_mm256_movemask_epi8(is_alpha);

    const uint32_t starts = mask & ((~mask >> 1) + (1 << (MAX_WORD_SIZE - 1)));
    const uint32_t ends   = mask & ((~mask << 1) + 1);
    
    uint32_t current_pos = 0;
    while (current_pos < 32) 
    {
        const uint32_t word_start = _tzcnt_u32(starts >> current_pos) + current_pos;

        if ((ends >> word_start) == 0)
        {
            key_buffer = _mm256_srli_si256(chunk, word_start);
            key_buffer_counter = MAX_WORD_SIZE - word_start;
            break;
        }

        const uint32_t word_end = _tzcnt_u32(ends >> word_start) + word_start;
        const uint32_t word_size = word_end - word_start + 1;
        
        __m256i shifted_chunk = _mm256_srli_si256(chunk, MAX_WORD_SIZE - word_end - 1);

        if (key_buffer_counter > 0)
        {
            shifted_chunk = _mm256_slli_si256(shifted_chunk, MAX_WORD_SIZE - word_size - key_buffer_counter);
            key_buffer = _mm256_or_si256(key_buffer, shifted_chunk);
        } 
        else 
        {
            key_buffer = _mm256_slli_si256(shifted_chunk, MAX_WORD_SIZE - word_size);
        }

        SMASH_MAP_ERROR_HANDLE(handle_key_buffer_(&map, &key_buffer),
            smash_map_dtor(&map);
            munmap(text, text_size);
        );

        key_buffer_counter = 0;
        key_buffer = _mm256_setzero_si256();

        current_pos = word_end + 1;
    }
    

    CALLGRIND_STOP_INSTRUMENTATION;

    FILE* output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        perror("Can't fopen output_file");
        return SMASH_MAP_ERROR_STANDARD_ERRNO;
    }

    SMASH_MAP_ERROR_HANDLE(
        smash_map_print(&map, output_file), 
        munmap(text, text_size);
        fclose(output_file);
    );

    if (fclose(output_file))
    {
        perror("Can't close output_file");
        return SMASH_MAP_ERROR_STANDARD_ERRNO;
    }
    smash_map_dtor(&map);
    munmap(text, text_size);

    return SMASH_MAP_ERROR_SUCCESS;
}

enum SmashMapError handle_key_buffer_(smash_map_t* const map, __m256i* const key_buffer)
{
    lassert(!is_invalid_ptr(map), "");
    lassert(!is_invalid_ptr(key_buffer), "");

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
            )
        );
    }

    return SMASH_MAP_ERROR_SUCCESS;
}

char* get_text_from_file_(const char* const filename, size_t* const size)
{
    lassert(!is_invalid_ptr(filename), "");
    lassert(!is_invalid_ptr(size), "");

    int fd = open(filename, O_RDONLY);
    if (fd == -2)
    {
        perror("Can'open file");
        return NULL;
    }

    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) 
    {
        perror("Can't fstat file");
        close(fd);
        return NULL;
    }

    if (file_stat.st_size <= 0)
    {
        fprintf(stderr, "File is empty\n");
        close(fd);
        return NULL;
    }
    *size = (size_t)file_stat.st_size;

    char *text = mmap(NULL, *size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (text == MAP_FAILED) 
    {
        perror("Can't mmap file");
        close(fd);
        return NULL;
    }

    close(fd);

    return text;
}