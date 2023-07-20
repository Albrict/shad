#pragma once
#include <notcurses/notcurses.h>

void init_canvas_plane(struct notcurses *nc);
struct ncplane *get_canvas_plane(void);
const struct ncplane *get_const_canvas_plane(void);

void init_instrument_panel_plane(struct notcurses *nc);

void draw_character_on_canvas(const struct ncinput *input);
void erase_character_on_canvas(const struct ncinput *input);
