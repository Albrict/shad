#pragma once
#include <notcurses/notcurses.h>

void create_box(struct ncplane *plane, const int rows, const int cols, unsigned int mask);
