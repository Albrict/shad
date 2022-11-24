#include "shad.h"

/* Error messages */
const char err_init_screen[] = "Coudln't initialize ncurses\n";
const char err_init_colors[] = "Coudln't initialize colors\n";
const char err_init_mouse[] = "Coudln't initialize mouse\n";

void die(const char *error)
{
    printf("%s", error);
    end_screen();
    exit(EXIT_FAILURE);
}
