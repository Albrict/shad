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
