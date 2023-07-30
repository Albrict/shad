#pragma once
#include <notcurses/notcurses.h>

struct ncplane *init_instrument_panel_plane(struct notcurses *nc);
void close_instument_panel(void);

void proccess_input_on_instrument_panel_plane(const struct ncinput *input, struct ncplane *instrument_panel);
