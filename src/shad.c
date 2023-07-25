#include <locale.h>

#include "instrument_panel.h"
#include "canvas.h"
#include "palette.h"
#include "shad_error.h"
#include "input.h"

static struct notcurses *nc = NULL;
static bool running = false;

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
    
    init_palette(nc);
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
        if (input.id == 'q')
            running = false;
        proccess_input(&input);
        notcurses_render(nc);
    }
}

void quit(void)
{
    delete_palette();
    notcurses_stop(nc);
}
