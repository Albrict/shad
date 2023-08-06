#include "state.h"

static uint32_t selected_color = 0;

uint32_t get_selected_color(void)
{
    return selected_color;
}

void select_color(const uint32_t color)
{
    selected_color = color;
}
