#include "shad.h"
#include <time.h>
#include <string.h>
#include <unistd.h>

/* Strings */
static char *name = NULL;

/* File functions */
static void load()
{
    FILE *f = fopen(name, "r"); 
    load_field_and_init(f);
    fclose(f);
}

static void create()
{
    FILE *f = fopen(name, "w");
    if (!f) {
        die("Cant't create file.");
    }
    fclose(f);
}

void save()
{
    FILE *f = fopen(name, "w");
    putwin(get_field(), f);
    fclose(f);
}

static void run()
{
    while (true) {
        procces_input();
        update();
    }
}

int main (int argc, char *argv[])
{
    /* Firstly we check, if we have arguments */
    if (argc > 1) {
        name = argv[1];
        /* Then we check, if file exist */
        if (access(name, F_OK) == 0) {
            load();
            run();
        } else {
            create();
            init_shad();
            run();
        }
    /* Else we create new file with time title */
    } else {
        time_t cur_time = time(NULL);
        char *current_time = ctime(&cur_time);
        name = current_time;
        current_time[strlen(current_time) - 1] = '\0';
        create();
        init_shad();
        run();
    }
    return 0;
}
