#include "logger/liblogger.h"
#include "smash_map/funcs/funcs.h"
#include "utils/utils.h"
#include "list_on_array/libfist.h"

#define FIST_ERROR_HANDLE_(call_func, ...)                                                          \
    do {                                                                                            \
        int error_handler = call_func;                                                              \
        if (error_handler)                                                                          \
        {                                                                                           \
            fprintf(stderr, "Can't " #call_func". Error: %s\n",                                     \
                            fist_strerror(error_handler));                                          \
            __VA_ARGS__                                                                             \
            return SMASH_MAP_ERROR_FIST;                                                            \
        }                                                                                           \
    } while(0)

#define FIST_CAPACITY 10
enum SmashMapError smash_map_ctor_NOT_USE(smash_map_t* const map, const size_t size,
                                          const size_t key_size, const size_t val_size,
                                          const char* const name, const place_in_code_t burn_place)
{
    lassert(!is_invalid_ptr(map), "");
    lassert(size, "");
    lassert(key_size, "");
    lassert(val_size, "");
    lassert(!is_invalid_ptr(name), "");

    IF_NDEBUG((void)name;)
    IF_NDEBUG((void)burn_place;)

    IF_DEBUG(map->burn_place = burn_place;)
    IF_DEBUG(map->name = name;)

    map->size = size;
    map->key_size = key_size;
    map->val_size = val_size;

    map->buckets = calloc(size, sizeof(*map->buckets));

    if (!map->buckets)
    {
        perror("Can't calloc map->buckets");
        return SMASH_MAP_ERROR_STANDARD_ERRNO;
    }

    for (size_t bucket_ind = 0; bucket_ind < map->size; ++bucket_ind)
    {
        FIST_ERROR_HANDLE_(
            FIST_CTOR(&map->buckets[bucket_ind], sizeof(smash_map_elem_t), FIST_CAPACITY),
            free(map->buckets);
            for (size_t ctored_bucket_ind = 0; ctored_bucket_ind < bucket_ind; ++ctored_bucket_ind)
            {
                fist_dtor(&map->buckets[ctored_bucket_ind]);
            }
        );
    }

    return SMASH_MAP_ERROR_SUCCESS;
}
#undef FIST_CAPACITY

void smash_map_dtor(smash_map_t* const map)
{
    lassert(!is_invalid_ptr(map), "");

    for (size_t bucket_ind = 0; bucket_ind < map->size; ++bucket_ind)
    {
        fist_dtor(&map->buckets[bucket_ind]);
    }
    
    free(map->buckets); IF_DEBUG(map->buckets = NULL);
}