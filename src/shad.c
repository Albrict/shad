#include <locale.h>

#include "instrument_panel.h"
#include "canvas.h"
#include "palette.h"
#include "shad_error.h"
#include "input.h"
#include "panel_list.h"

static struct notcurses *nc = NULL;
static bool running = false;

void init(void)
{

    const char *error_init_message  = "Can't initialize notcurses!\n";
    const char *error_mouse_message = "Can't initialize mouse!\n";
    const char *error_canvas_init_message = "Can't create canvas!\n";
    const char *error_instrument_panel_init_message = "Can't create instrument_panel!\n";

    setlocale(LC_ALL, "");
    nc = notcurses_init(NULL, stdout);
    if (nc == NULL) 
        die(error_init_message);
    init_error_system(nc);    

    int res = notcurses_mice_enable(nc, 1);
    if (res < 0)
        die_and_log(error_mouse_message);

    init_palette(nc);

    struct ncplane *canvas = init_canvas_plane(nc);
    if (canvas == NULL)
        die_and_log(error_canvas_init_message);
    else
        add_panel_to_list(canvas, proccess_input_on_canvas);

    struct ncplane *instrument_panel = init_instrument_panel_plane(nc);
    if (instrument_panel == NULL)
        die_and_log(error_instrument_panel_init_message);
    else
        add_panel_to_list(instrument_panel, proccess_input_on_instrument_panel_plane);

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
    free_panel_list();
    close_instument_panel();
    delete_palette();
    notcurses_stop(nc);
}
