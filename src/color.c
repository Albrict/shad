#include "color.h"

static uint32_t current_color = 0;

void set_current_color(const uint32_t new_color)
{
    current_color = new_color;
}

uint32_t get_current_color(void)
{
    return current_color; 
}
