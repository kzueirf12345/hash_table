#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "test/test.h"
#include "utils/utils.h"
#include "logger/liblogger.h"
#include "smash_map/funcs/funcs.h"
#include "smash_map/verification/verification.h"
#include "test/utils.h"

char* get_text_from_file_(const char* const filename, size_t* const size);

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

    smash_map_t map = {};
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

    char   key_buffer[MAX_WORD_SIZE] = {};
    size_t key_buffer_counter            = 0;
    for (size_t chr_ind = 0; chr_ind < text_size; ++chr_ind)
    {
        const char chr = text[chr_ind];
        if (isalpha(chr))
        {
            IF_DEBUG(
                if (key_buffer_counter >= MAX_WORD_SIZE)
                {
                    fprintf(stderr, "Founded word with size more then max valid\n");
                    smash_map_dtor(&map);
                    munmap(text, text_size);
                    return SMASH_MAP_ERROR_UNKNOWN;
                }
            )
            key_buffer[key_buffer_counter++] = chr;
        }
        else
        {
            if (!key_buffer_counter)
            {
                continue;
            }

            size_t* val = smash_map_get_val(&map, key_buffer);
            if (val)
            {
                ++*val;
            }
            else
            {
                size_t temp_val = 1;
                SMASH_MAP_ERROR_HANDLE(
                    smash_map_insert(&map, (smash_map_elem_t){.key = key_buffer, .val = &temp_val}),
                    smash_map_dtor(&map);
                    munmap(text, text_size);
                );
            }

            key_buffer_counter = 0;
            if (!memset(key_buffer, 0, MAX_WORD_SIZE))
            {
                perror("Can't memset key_buffer");
                return SMASH_MAP_ERROR_STANDARD_ERRNO;
            }
            
        }
    }

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