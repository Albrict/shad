#pragma once
#include "ncpanel.h"

struct ncpanel *create_canvas_panel(struct ncplane *parent, const char *filename, const unsigned int rows, const unsigned int cols);
