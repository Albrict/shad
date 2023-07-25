#pragma once
#include <notcurses/notcurses.h>

void init_canvas_plane(struct notcurses *nc);
void proccess_input_on_canvas(const struct ncinput *input);
struct ncplane *get_canvas_plane(void);
const struct ncplane *get_const_canvas_plane(void);
