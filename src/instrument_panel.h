#pragma once
#include <notcurses/notcurses.h>

void init_instrument_panel_plane(struct notcurses *nc);
void proccess_input_on_instrument_panel_plane(const struct ncinput *input);

struct ncplane *get_instument_panel_plane(void);
const struct ncplane *get_const_instument_panel_plane(void);
