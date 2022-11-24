#include <ncurses.h>
#include <stdlib.h>
enum keys {
    KEY_ESC = 27
};

/* System */
void die(const char *error);

/* Graphics */
WINDOW *get_field();
WINDOW *get_bar();

void update();
void init_shad();
void end_screen();

/* Controls */
void procces_input();

/* Color */
void init_shad_colors();
void change_color(const size_t new_color);
size_t get_current_color();
