#pragma once
#include <notcurses/notcurses.h>

struct ncplane *init_canvas_plane(struct notcurses *nc);
void proccess_input_on_canvas(const struct ncinput *input, struct ncplane *canvas);

/* After locking canvas - user can't draw on it */
void lock_canvas(void);
/* After unlocking canvas - user can draw on it */
void unlock_canvas(void);
