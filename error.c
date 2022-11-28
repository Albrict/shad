#include "shad.h"

/* Error messages */
const char err_init_screen[] = "Coudln't initialize ncurses\n";
const char err_init_colors[] = "Coudln't initialize colors\n";
const char err_init_mouse[] = "Coudln't initialize mouse\n";
const char err_open_drawing[] = "Coudln't open file. File is too large.\n";

void die(const char *error)
{
    end_screen();
    printf("%s", error);
    exit(EXIT_FAILURE);
}
