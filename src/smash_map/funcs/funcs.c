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


#define FIST_CAPACITY 10
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
            for (size_t ctored_bucket_ind = 0; ctored_bucket_ind < bucket_ind; ++ctored_bucket_ind)
            {
                fist_dtor(&map->buckets[ctored_bucket_ind].keys);
                fist_dtor(&map->buckets[ctored_bucket_ind].vals);
            }
            free(map->buckets);
        );

        FIST_ERROR_HANDLE_VALS_(
            FIST_CTOR(&map->buckets[bucket_ind].vals, val_size, FIST_CAPACITY),
            for (size_t ctored_bucket_ind = 0; ctored_bucket_ind < bucket_ind; ++ctored_bucket_ind)
            {
                fist_dtor(&map->buckets[ctored_bucket_ind].keys);
                fist_dtor(&map->buckets[ctored_bucket_ind].vals);
            }
            fist_dtor(&map->buckets[bucket_ind].keys);
            free(map->buckets);
        );
    }

    return SMASH_MAP_ERROR_SUCCESS;
}
#undef FIST_CAPACITY

void smash_map_dtor(smash_map_t* const map)
{
    SMASH_MAP_VERIFY_ASSERT(map, map->key_to_str, map->val_to_str);

    for (size_t bucket_ind = 0; bucket_ind < map->size; ++bucket_ind)
    {
        fist_dtor(&map->buckets[bucket_ind].keys);
        fist_dtor(&map->buckets[bucket_ind].vals);
    }
    
    free(map->buckets); IF_DEBUG(map->buckets = NULL);
}

enum SmashMapError smash_map_insert(smash_map_t* const map, const smash_map_elem_t elem)
{
    SMASH_MAP_VERIFY_ASSERT(map, map->key_to_str, map->val_to_str);
    lassert(!is_invalid_ptr(elem.key), "");
    lassert(!is_invalid_ptr(elem.val), "");

    const size_t bucket_ind = map->hash_func(elem.key) % map->size;
    // fprintf(stderr, RED_TEXT("kbucket_ind: %zu\n"), bucket_ind);

    size_t finded_ind = fist_find(&map->buckets[bucket_ind].keys, elem.key);

    if (!finded_ind)
    {
        FIST_ERROR_HANDLE_KEYS_(fist_push(&map->buckets[bucket_ind].keys, 0, elem.key));
        FIST_ERROR_HANDLE_VALS_(fist_push(&map->buckets[bucket_ind].vals, 0, elem.val));

        // fprintf(stderr, RED_TEXT("keys_size: %zu\n"), map->buckets[bucket_ind].keys.size);
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
    SMASH_MAP_VERIFY_ASSERT(map, map->key_to_str, map->val_to_str);
    lassert(!is_invalid_ptr(key), "");

    const size_t bucket_ind = map->hash_func(key) % map->size;
    const size_t finded_ind = fist_find(&map->buckets[bucket_ind].keys, key);

    if (!finded_ind) 
        return NULL;

    return map->buckets[bucket_ind].vals.data + finded_ind * map->buckets[bucket_ind].vals.elem_size;
}