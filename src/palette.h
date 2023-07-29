#pragma once
#include <notcurses/notcurses.h>

void init_palette(struct notcurses *nc);
void delete_palette(void);

struct ncpalette *get_palette(void);
int get_amount_of_colors(void);
