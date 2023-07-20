#include <locale.h>
#include "screen.h"
#include "shad_error.h"

static struct notcurses *nc = NULL;
static bool running = false;

static void proccess_input(const ncinput *input);

void init(void)
{
    const char *error_init_message  = "Can't initialize notcurses!\n";
    const char *error_mouse_message = "Can't initialize mouse!\n";

    setlocale(LC_ALL, "");
    nc = notcurses_init(NULL, stdout);
    if (nc == NULL) 
        die_and_log(error_init_message);

    int res = notcurses_mice_enable(nc, 1);
    if (res < 0)
        die_and_log(error_mouse_message);
    
    init_canvas_plane(nc);
    init_instrument_panel_plane(nc);
    notcurses_render(nc);
    running = true;
}

void run(void)
{
    struct ncinput input;
    memset(&input, 0, sizeof(struct ncinput));
    
    while (running) {
        notcurses_get_blocking(nc, &input);
        proccess_input(&input);
        notcurses_render(nc);
    }
}

void quit(void)
{
    notcurses_stop(nc);
}

static void proccess_input(const ncinput *input)
{
    switch(input->id) {
    case NCKEY_BUTTON1:
        draw_character_on_canvas(input);
        break;
    case NCKEY_BUTTON3:
        erase_character_on_canvas(input);
        break;
    case 'q':
        running = false;
        break;
    }
}
