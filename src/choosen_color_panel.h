#pragma once
#include <notcurses/notcurses.h>

void init_choosen_color_panel(struct ncplane *parent, const int y, const int x, const int rows, const int cols, const uint32_t mask);

void set_color(const uint32_t new_color);
const uint32_t get_choosen_color(void);
