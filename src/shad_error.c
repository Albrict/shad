#include "shad_error.h"

struct notcurses *notcurses = NULL;
void init_error_system(struct notcurses *nc)
{
    notcurses = nc;
}

void die_and_log(const char *message)
{
    notcurses_stop(notcurses);
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
}

void die(const char *message)
{
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
}

void panic_and_log(const char *message)
{
    ;
}
