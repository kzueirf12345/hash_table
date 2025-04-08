#include "utils/utils.h"
#include "smash_map/dumb/dumb.h"
#include "smash_map/structs.h"
#include "logger/liblogger.h"
#include "list_on_array/libfist.h"

static const char* const HTML_INTRO_ =
    "\n<!DOCTYPE html>\n"
    "<html lang='en'>\n"
        "<head>\n"
            "<meta charset='UTF-8'>\n"
            "<meta http-equiv='X-UA-Compatible' content='IE=edge'>\n"
            "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
            "<title>MEGA MEGA MEGA DUMB</title>\n"
        "</head>\n"
        "<body>\n"
            "<pre>\n";

#define DUMB_AND_FPRINTF_(format, ...)                                                              \
    do {                                                                                            \
        fprintf((*DUMBER_get_html_file()), format, ##__VA_ARGS__);                                  \
        fprintf(stderr,                    format, ##__VA_ARGS__);                                  \
    } while(0)

void smash_map_dumb_NOT_USE(const smash_map_t* const map, const place_in_code_t call_place, 
                            elem_to_str_t elem_to_str)
{
    if (fist_is_empty_file((*DUMBER_get_html_file())) <= 0) 
        fprintf((*DUMBER_get_html_file()), HTML_INTRO_);

    fprintf((*DUMBER_get_html_file()), "</pre><hr /><pre>\n");  
    fprintf((*DUMBER_get_html_file()), "</pre><hr /><pre>\n"); 

    DUMB_AND_FPRINTF_("\n==SMASH MAP DUMB==\nDate: %s\nTime: %s\n\n", __DATE__, __TIME__);

    const char*  map_buf  = handle_invalid_ptr(map);
    const char* file_buf  = handle_invalid_ptr(call_place.file);
    const char* func_buf  = handle_invalid_ptr(call_place.func);
    file_buf =  file_buf  ? file_buf :         call_place.file;
    func_buf =  func_buf  ? func_buf :         call_place.func;
    const int   line_buf  =                    call_place.line <= 0
                                               ? CODE_LINE_POISON
                                               : call_place.line;

    if (map_buf)
    {
        DUMB_AND_FPRINTF_("smash_map_t [%s] at %s:%d (%s())\n", map_buf, file_buf, line_buf, func_buf);
        fprintf(stderr, "\n");
        return;
    }

    const char*           map_name_buf      = handle_invalid_ptr( map->name           );
    const char*           map_file_burn_buf = handle_invalid_ptr( map->burn_place.file);
    const char*           map_func_burn_buf = handle_invalid_ptr( map->burn_place.func);
    map_name_buf        = map_name_buf      ? map_name_buf      : map->name;
    map_file_burn_buf   = map_file_burn_buf ? map_file_burn_buf : map->burn_place.file;
    map_func_burn_buf   = map_func_burn_buf ? map_func_burn_buf : map->burn_place.func;
    const int             map_line_burn_buf = map->burn_place.line <= 0
                                              ? CODE_LINE_POISON
                                              : map->burn_place.line;


    DUMB_AND_FPRINTF_("smash_map_t %s[%p] at %s:%d (%s()) bUUUrn at %s:%d (%s())\n",
                       map_name_buf, map,
                       file_buf, line_buf, func_buf,
                       map_file_burn_buf, map_line_burn_buf, map_func_burn_buf);

    DUMB_AND_FPRINTF_("\n");
    DUMB_AND_FPRINTF_("\tsize      = %zu\n",   map->size);
    DUMB_AND_FPRINTF_("\tkey_size  = %zu\n",   map->key_size);
    DUMB_AND_FPRINTF_("\tval_size  = %zu\n\n", map->val_size);

    DUMB_AND_FPRINTF_("\thash_func = %p\n",    map->hash_func);
    DUMB_AND_FPRINTF_("\tbuckets   = %p\n",    map->buckets);
    DUMB_AND_FPRINTF_("\n");

    for (size_t bucket_ind = 0; bucket_ind < map->size; ++bucket_ind)
    {
        DUMB_AND_FPRINTF_("BUCKET INDEX: %zu\n", bucket_ind);
        FIST_DUMB(&map->buckets[bucket_ind], elem_to_str);
    }
}