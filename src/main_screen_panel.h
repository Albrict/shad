#pragma once
#include "ncpanel.h"
struct shad_opts;

struct ncpanel *create_main_screen_panel(struct notcurses *nc, bool *running, const struct shad_opts *opts);
