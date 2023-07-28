#pragma once
#include <notcurses/notcurses.h>

struct button;

typedef void (*button_callback)(void *);
/* Creates button by using ncplane  and stores it into a list 
 * On failure returns false */
struct button *create_button(struct ncplane *parent, const int y, const int x, const unsigned int height, const unsigned int width);

void bind_callback(struct button *button, button_callback callback, void *data);
void proccess_input_on_button(struct button *button, const ncinput *input);

/* Free button and delete it from list */
void delete_button(struct button *button);
void draw_button_box(struct button *button, unsigned int mask);
void draw_text_on_button(struct button *button, const wchar_t *text);
