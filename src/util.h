#pragma once
#include <notcurses/notcurses.h>

void create_box(struct ncplane *plane, const int rows, const int cols, unsigned int mask);
bool blit_image_on_plane(const char *filename, struct notcurses *nc, struct ncplane *plane);
