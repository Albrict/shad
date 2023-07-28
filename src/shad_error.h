#pragma once
#include <notcurses/notcurses.h>

/* Save pointer to properly close program if error occures */
void init_error_system(struct notcurses *nc);
/* Stoping notcurses and logs */
void die_and_log(const char *message);
/* Just exit program and log in stdout */
void die(const char *message);
/* Log error/warning into a file, but continue working */
void panic_and_log_in_file(const char *message);
