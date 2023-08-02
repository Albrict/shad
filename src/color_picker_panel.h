#pragma once
#include <notcurses/notcurses.h>

struct ncplane *create_color_picker_panel(struct ncplane *parent, const int y, const int x, const int rows, const int cols);
void proccess_input_on_color_picker(const struct ncinput *input, struct ncplane *color_picker);
