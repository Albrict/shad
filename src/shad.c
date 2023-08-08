#include <locale.h>

#include "main_screen_panel.h"
#include "palette.h"
#include "shad_error.h"
#include "ncpanel.h"

static struct notcurses *nc = NULL;
static struct ncpanel *main_screen = NULL;
static bool running = false;

void init(char *argv)
{
    const char *error_init_message  = "Can't initialize notcurses!\n";
    const char *error_mouse_message = "Can't initialize mouse!\n";
    const char *error_main_screen_message = "Can't initialize main screen!\n";

    setlocale(LC_ALL, "");
    nc = notcurses_init(NULL, stdout);
    if (nc == NULL) 
        die(error_init_message);
    init_error_system(nc);    

    int res = notcurses_mice_enable(nc, 1);
    if (res < 0)
        die_and_log(error_mouse_message);

    init_palette(nc);
    
    main_screen = create_main_screen_panel(nc, &running, argv);
    if (!main_screen)
        die_and_log(error_main_screen_message);
    
    notcurses_render(nc);
    running = true;
}

void run(void)
{
    struct ncinput input;
    memset(&input, 0, sizeof(struct ncinput));
    
    while (running) {
        notcurses_get_blocking(nc, &input);
        if (input.id == 'q')
            running = false;
        ncpanel_proccess_input_and_update(main_screen, &input);
        notcurses_render(nc);
    }
}

void quit(void)
{
    ncpanel_destroy(main_screen);
    delete_palette();
    notcurses_stop(nc);
}
