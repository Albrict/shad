#pragma once
#include <notcurses/notcurses.h>

struct ncplane *create_palette_window(struct ncplane *parent, bool *is_opened);
void proccess_input_on_palette_window(const struct ncinput *input, struct ncplane *palette_window);
