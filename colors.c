#include "shad.h"

typedef enum {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
} colors;

static colors current_color = BLACK;

/* Initialization functions */
void init_shad_colors()
{
    colors i;
    for (i = 0; i < WHITE + 1; i++)
        init_pair(i, i, i);
}

/* Color functions */
size_t get_current_color()
{
    return current_color;
}

void change_color(const size_t new_color)
{
    wattroff(get_field(), COLOR_PAIR(current_color));
    current_color = new_color; 
    wattron(get_field(), COLOR_PAIR(current_color));
}
