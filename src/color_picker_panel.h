#pragma once
#include <notcurses/notcurses.h>

void init_color_picker(struct ncplane *parent, const int y, const int x, const int rows, const int cols);

uint32_t get_selected_color(const struct ncinput *input);
bool is_mouse_on_color_picker(const int y, const int x);
