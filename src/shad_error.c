#include "shad_error.h"
#include <notcurses/notcurses.h>

void die_and_log(const char *message)
{
    notcurses_stop(NULL);
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
}

void panic_and_log(const char *message)
{
    ;
}
