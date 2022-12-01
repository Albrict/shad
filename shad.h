#include <ncurses.h>
#include <stdlib.h>
#ifndef SHAD_H
#define SHAD_H

typedef enum {
    BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
} colors;

typedef enum {
    FONT_BLACK
} special_colors;

/* System */
void die(const char *error);
void save();

/* Graphics */
WINDOW *get_field();
WINDOW *get_bar();

void update();
void init_shad();
void load_field_and_init(FILE *field);
void end_screen();

void set_current_color();
/* Controls */
void procces_input();

/* Color */
void init_shad_colors();
void change_color(const colors new_color, WINDOW *window);
colors get_current_color();

#endif /* SHAD_H */
