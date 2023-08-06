#include <locale.h>

#include "instrument_panel.h"
#include "canvas.h"
#include "palette.h"
#include "shad_error.h"
#include "ncpanel.h"
#include "util.h"

static struct notcurses *nc = NULL;
static struct ncpanel_list *core_panels = NULL;
static bool running = false;

static void proccess_input(struct ncpanel_list *core_panels, const struct ncinput *input);

void init(const char *argv)
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

    /* Create core panels and put them into a list */
    core_panels = ncpanel_create_list();

    struct ncpanel *canvas = create_canvas_panel(notcurses_stdplane(nc));
    if (!canvas)
        die_and_log(error_canvas_init_message);
    else
        ncpanel_add_panel_to_list(core_panels, canvas);
    
    if (argv)
        blit_image_on_plane(argv, nc, ncpanel_get_plane(canvas));

    struct ncpanel *instrument_panel = create_instrument_panel(notcurses_stdplane(nc));
    if (!instrument_panel)
        die_and_log(error_instrument_panel_init_message);
    else
        ncpanel_add_panel_to_list(core_panels, instrument_panel);

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
        proccess_input(core_panels, &input);
        notcurses_render(nc);
    }
}

void quit(void)
{
    ncpanel_destroy_list(core_panels);
    delete_palette();
    notcurses_stop(nc);
}

static void proccess_input(struct ncpanel_list *core_panels, const struct ncinput *input)
{
    struct ncpanel_node *first = ncpanel_list_begin(core_panels);
    while (first) {
        ncpanel_proccess_input(ncpanel_get_panel(first), input);
        first = ncpanel_list_next(first);
    }
}
