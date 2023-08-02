#include "libs/cJSON.h"

#include "palette.h"
#include "shad_error.h"

static struct ncpalette *current_pallete = NULL;
static int color_amount = 0;

static void parse_json_and_set_colors(void);

void init_palette(struct notcurses *nc)
{
    const char *error_palette_create = "Can't create palette!\n";
    current_pallete = ncpalette_new(nc);
    if (current_pallete == NULL)
        die_and_log(error_palette_create);
     
    parse_json_and_set_colors();
    ncpalette_use(nc, current_pallete);
}

void delete_palette(void)
{
    ncpalette_free(current_pallete);
}

struct ncpalette *get_palette(void)
{
    return current_pallete;
}

int get_amount_of_colors(void)
{
    return color_amount;
}

/* Open json file with palette settings and read it */
/* into a buffer */
static void parse_json_and_set_colors(void)
{
    const char *path = "../palette/default-palette.json";
    const char *error_palette_file_open = "Can't find palette file!\n";
    cJSON *color = NULL;
    cJSON *palette = NULL;
    cJSON *json_file = NULL;
    FILE *f = NULL;

    f = fopen(path, "r");
    if (f == NULL)
        die_and_log(error_palette_file_open);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    
    char *buffer = malloc(size + 1);
    fread(buffer, size, 1, f);
    fclose(f);
    
    buffer[size] = '\0';
     
    json_file = cJSON_Parse(buffer);
    if (json_file == NULL) {
        free(buffer);
        die_and_log(cJSON_GetErrorPtr());
    }

    palette = cJSON_GetObjectItemCaseSensitive(json_file, "palette");
    if (palette == NULL) {
        free(buffer);
        cJSON_Delete(json_file);
        die_and_log(cJSON_GetErrorPtr());
    }

    cJSON_ArrayForEach(color, palette) {
        const cJSON *hex_value = cJSON_GetObjectItemCaseSensitive(color, "hexString"); 
        const cJSON *id = cJSON_GetObjectItemCaseSensitive(color, "colorId"); 

        int color_id = id->valueint;

        /* Getting raw hex string and convert it into actual hex value */
        char *str = hex_value->valuestring;
        memmove(str, str + 1, strlen(str));
        uint32_t color = strtol(str, NULL, 16);
        ncpalette_set(current_pallete, color_id, color);
        ++color_amount;
    }

    free(buffer);
    cJSON_Delete(json_file);
}
