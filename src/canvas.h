#pragma once
#include "ncpanel.h"

struct ncpanel *create_canvas_panel(struct ncplane *parent);

/* After locking canvas - user can't draw on it */
void lock_canvas(void);
/* After unlocking canvas - user can draw on it */
void unlock_canvas(void);
