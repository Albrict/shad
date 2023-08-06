#pragma once
#include "ncpanel.h"

struct ncpanel *create_selected_color_panel(struct ncplane *parent, const int y, const int x, 
                                                  const int rows, const int cols, const uint32_t mask);
