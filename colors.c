#include "shad.h"

static colors current_color = BLACK;

/* Initialization functions */
void init_shad_colors()
{
    colors i;
    for (i = 0; i < WHITE + 1; i++)
        init_pair(i, i, i);
    init_pair(FONT_BLACK, COLOR_WHITE, COLOR_BLACK);
}

/* Color functions */
colors get_current_color()
{
    return current_color;
}

void change_color(const colors new_color, WINDOW *window)
{
    wattroff(window, COLOR_PAIR(current_color));
    current_color = new_color; 
    wattron(window, COLOR_PAIR(current_color));
}
