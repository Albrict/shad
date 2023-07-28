#pragma once
#include <notcurses/notcurses.h>

struct label;

/* Create label by using ncplane */
struct label *create_label(struct ncplane *parent, const int y, const int x, 
                           const unsigned int height, const unsigned int width, const char *text);
struct label *create_label_without_ncplane(struct ncplane *parent, const int y, const int x, 
                                           const unsigned int height, const unsigned int width, const char *text);
void delete_label(struct label *label);

/* You can call this function's once */
void draw_label_box(struct label *label, unsigned int mask);
void draw_label_and_box(struct label *label, unsigned int mask);
void draw_label(struct label *label);
