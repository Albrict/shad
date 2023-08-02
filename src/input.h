#pragma once
#include <notcurses/notcurses.h>
#include "panel_list.h"

void proccess_input(struct panel_list *core_panels, const struct ncinput *input);
