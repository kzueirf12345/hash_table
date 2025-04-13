#include "logger/liblogger.h"
#include "smash_map/funcs/funcs.h"
#include "utils/utils.h"
#include "list_on_array/libfist.h"
#include "smash_map/verification/verification.h"

#define FIST_ERROR_HANDLE_KEYS_(call_func, ...)                                                     \
    do {                                                                                            \
        int error_handler = call_func;                                                              \
        if (error_handler)                                                                          \
        {                                                                                           \
            fprintf(stderr, "Can't " #call_func". Error: %s\n",                                     \
                            fist_strerror(error_handler));                                          \
            __VA_ARGS__                                                                             \
            return SMASH_MAP_ERROR_FIST_KEYS;                                                       \
        }                                                                                           \
    } while(0)

#define FIST_ERROR_HANDLE_VALS_(call_func, ...)                                                     \
    do {                                                                                            \
        int error_handler = call_func;                                                              \
        if (error_handler)                                                                          \
        {                                                                                           \
            fprintf(stderr, "Can't " #call_func". Error: %s\n",                                     \
                            fist_strerror(error_handler));                                          \
            __VA_ARGS__                                                                             \
            return SMASH_MAP_ERROR_FIST_VALS;                                                       \
        }                                                                                           \
    } while(0)


void smash_map_dtor_fists_(smash_map_t* const map, const size_t fists_cnt);

#define FIST_CAPACITY 15
enum SmashMapError smash_map_ctor_NOT_USE(smash_map_t* const map, const size_t size,
                                          const size_t key_size, const size_t val_size,
                                          hash_func_t hash_func,
                                          elem_to_str_t key_to_str, elem_to_str_t val_to_str,
                                          const char* const name, const place_in_code_t burn_place)
{
    lassert(!is_invalid_ptr(map), "");
    lassert(size, "");
    lassert(!is_invalid_ptr(name), "");
    lassert(!is_invalid_ptr(hash_func), "");

    IF_NDEBUG((void)name;)
    IF_NDEBUG((void)burn_place;)

    IF_DEBUG(map->burn_place = burn_place;)
    IF_DEBUG(map->name = name;)

    map->size           = size;
    map->hash_func      = hash_func;
    map->key_to_str     = key_to_str;
    map->val_to_str     = val_to_str;

    map->buckets = calloc(size, sizeof(*map->buckets));

    if (!map->buckets)
    {
        perror("Can't calloc map->buckets");
        return SMASH_MAP_ERROR_STANDARD_ERRNO;
    }

    for (size_t bucket_ind = 0; bucket_ind < map->size; ++bucket_ind)
    {
        FIST_ERROR_HANDLE_KEYS_(
            FIST_CTOR(&map->buckets[bucket_ind].keys, key_size, FIST_CAPACITY),
            smash_map_dtor_fists_(map, bucket_ind);
            free(map->buckets);
        );

        FIST_ERROR_HANDLE_VALS_(
            FIST_CTOR(&map->buckets[bucket_ind].vals, val_size, FIST_CAPACITY),
            smash_map_dtor_fists_(map, bucket_ind);
            fist_dtor(&map->buckets[bucket_ind].keys);
            free(map->buckets);
        );
    }

    return SMASH_MAP_ERROR_SUCCESS;
}
#undef FIST_CAPACITY

void smash_map_dtor(smash_map_t* const map)
{
    SMASH_MAP_VERIFY_ASSERT(map);

    smash_map_dtor_fists_(map, map->size);
    
    free(map->buckets); IF_DEBUG(map->buckets = NULL);
}

void smash_map_dtor_fists_(smash_map_t* const map, const size_t fists_cnt)
{
    lassert(!is_invalid_ptr(map), "");

    for (size_t bucket_ind = 0; bucket_ind < fists_cnt; ++bucket_ind)
    {
        fist_dtor(&map->buckets[bucket_ind].keys);
        fist_dtor(&map->buckets[bucket_ind].vals);
    }
}

enum SmashMapError smash_map_insert(smash_map_t* const map, const smash_map_elem_t elem)
{
    SMASH_MAP_VERIFY_ASSERT(map);
    lassert(!is_invalid_ptr(elem.key), "");
    lassert(!is_invalid_ptr(elem.val), "");

    const size_t bucket_ind = map->hash_func(elem.key) % map->size;
    const size_t finded_ind = fist_find(&map->buckets[bucket_ind].keys, elem.key);

    if (!finded_ind)
    {
        FIST_ERROR_HANDLE_KEYS_(fist_push(&map->buckets[bucket_ind].keys, 0, elem.key));
        FIST_ERROR_HANDLE_VALS_(fist_push(&map->buckets[bucket_ind].vals, 0, elem.val));
    }
    else
    {
        void* finded_val = (char*)map->buckets[bucket_ind].vals.data 
                         + finded_ind * map->buckets[bucket_ind].vals.elem_size;

        if (!memcpy(finded_val, elem.val, map->buckets[bucket_ind].vals.elem_size))
        {
            perror("Can't memcpy val");
            return SMASH_MAP_ERROR_STANDARD_ERRNO;
        }
    }

    return SMASH_MAP_ERROR_SUCCESS;
}

void* smash_map_get_val(smash_map_t* const map, const void* const key)
{
    SMASH_MAP_VERIFY_ASSERT(map);
    lassert(!is_invalid_ptr(key), "");

    const size_t bucket_ind = map->hash_func(key) % map->size;
    const size_t finded_ind = fist_find(&map->buckets[bucket_ind].keys, key);

    if (!finded_ind) 
        return NULL;

    return (char*)map->buckets[bucket_ind].vals.data 
                + finded_ind * map->buckets[bucket_ind].vals.elem_size;
}

#define MAX_ELEM_STR_SIZE 256
enum SmashMapError smash_map_print(const smash_map_t* const map, FILE* const file)
{
    SMASH_MAP_VERIFY_ASSERT(map);
    lassert(!is_invalid_ptr(file), "");

    char* elem_str = calloc(MAX_ELEM_STR_SIZE + 1, sizeof(*elem_str));

    if (!elem_str)
    {
        perror("Can't calloc elem_str");
        return SMASH_MAP_ERROR_STANDARD_ERRNO;
    }

    for (size_t bucket_ind = 0; bucket_ind < map->size; ++bucket_ind)
    {
        const fist_t keys = map->buckets[bucket_ind].keys;
        const fist_t vals = map->buckets[bucket_ind].vals;

        for (size_t elem_ind = keys.next[0]; elem_ind != 0; elem_ind = keys.next[elem_ind])
        {
            INT_ERROR_HANDLE(
                map->key_to_str(
                    (char*)keys.data + elem_ind * keys.elem_size, 
                    keys.elem_size, 
                    &elem_str, 
                    MAX_ELEM_STR_SIZE
                )
            );

            if (fprintf(file, "%s: ", elem_str) <= 0)
            {
                perror("Can't fprintf elem_str key");
                return SMASH_MAP_ERROR_STANDARD_ERRNO;
            }

            INT_ERROR_HANDLE(
                map->val_to_str(
                    (char*)vals.data + elem_ind * vals.elem_size, 
                    vals.elem_size, 
                    &elem_str, 
                    MAX_ELEM_STR_SIZE
                )
            );

            if (fprintf(file, "%s\n", elem_str) <= 0)
            {
                perror("Can't fprintf elem_str val");
                return SMASH_MAP_ERROR_STANDARD_ERRNO;
            }
        }
    }

    free(elem_str); IF_DEBUG(elem_str = NULL);

    return SMASH_MAP_ERROR_SUCCESS;
}
#undef MAX_ELEM_STR_SIZE