#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    const char *path = "default-palette.json";
    const cJSON *color = NULL;
    const cJSON *palette = NULL;

    FILE *f = NULL;
    f = fopen(path, "r");
    if (f == NULL)
        exit(1);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    
    char *buffer = (char*)malloc(size + 1);
    fread(buffer, size, 1, f);
    fclose(f);
    
    buffer[size] = '\0';
    
    cJSON *file_json = cJSON_Parse(buffer);
    if (file_json == NULL)
        exit(1);
    else 
        printf("Success\n");
    
    palette = cJSON_GetObjectItemCaseSensitive(file_json, "palette");
    cJSON_ArrayForEach(color, palette) {
        cJSON *hex_value = cJSON_GetObjectItemCaseSensitive(color, "hexString"); 
        cJSON *color_id = cJSON_GetObjectItemCaseSensitive(color, "colorId"); 
        char *str = hex_value->valuestring;
        int id = color_id->valueint;
        memmove(str, str + 1, strlen(str));
        printf("%s\n", str);
        printf("%d\n", id);
    
    }
    return 0;
}
