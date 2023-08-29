#pragma once
#include "ncpanel.h"

struct ncpanel *create_main_screen_panel(struct notcurses *nc, bool *running, const char *filename);
