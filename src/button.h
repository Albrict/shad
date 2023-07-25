#pragma once
#include <notcurses/notcurses.h>

struct button;

/* Creates button by using ncplane */
struct button *create_button(struct ncplane *parent, const int y, const int x, const unsigned int height, const unsigned int width);

/* Delete button from parent plane */
void delete_button(struct button *button);

void draw_box(struct button *button, unsigned int mask);
