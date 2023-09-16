#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "canvas.h"
#include "instrument_panel.h"
#include "menu.h"
#include "palette.h"
#include "shad_error.h"
#include "ncpanel.h"

struct shad_opts {
    unsigned int canvas_width;
    unsigned int canvas_height;
    const char *filename;
};

static struct notcurses *nc = NULL;

static struct shad_opts parse_arguments(const int argc, const char **argv);
static void init(const struct shad_opts);
static void init_ncpanels(const struct shad_opts opts);
static void quit();

int main(const int argc, const char **argv)
{
    const struct shad_opts opts = parse_arguments(argc, argv);
    init(opts);
    ncpanel_run(nc);
    quit();
}

static void init(const struct shad_opts opts)
{
    const char *error_init_message  = "Can't initialize notcurses!\n";
    const char *error_mouse_message = "Can't initialize mouse!\n";
    const char *error_main_screen_message = "Can't initialize main screen!\n";

    setlocale(LC_ALL, "");
    nc = notcurses_init(NULL, stdout);
    if (!nc) 
        die(error_init_message);
    init_error_system(nc);    

    if (notcurses_mice_enable(nc, 1) < 0)
        die_and_log(error_mouse_message);
    
    init_palette(nc);
    ncpanel_init(nc);
    init_ncpanels(opts); 
    notcurses_render(nc);
}

static void init_ncpanels(const struct shad_opts opts)
{
    struct ncpanel *canvas = NULL;
    struct ncpanel *instrument_panel = NULL;
    unsigned int terminal_rows, terminal_cols;
    notcurses_term_dim_yx(nc, &terminal_rows, &terminal_cols);

    unsigned int canvas_rows = terminal_rows - 2;
    unsigned int canvas_cols = terminal_cols / 2 + terminal_cols / 4;
    
    if (opts.canvas_height > 0)
        canvas_rows = opts.canvas_height;
    if (opts.canvas_width > 0)
        canvas_cols = opts.canvas_width;     

    canvas = create_canvas_panel(opts.filename, canvas_rows, canvas_cols);
    if (!canvas)
        ncpanel_quit();
    instrument_panel = create_instrument_panel(canvas);
    if (!instrument_panel)
        ncpanel_quit();
    
    if (opts.filename)
        ncpanel_blit_image_from_file(opts.filename, canvas, NCBLIT_1x1);
    return;
instrument_panel_err:
    ncpanel_destroy(instrument_panel);
canvas_err:
    ncpanel_destroy(canvas);
    return;
}

static void quit(void)
{
    ncpanel_quit();
    delete_palette();
    notcurses_stop(nc);
}

static struct shad_opts parse_arguments(const int argc, const char **argv)
{

    const char *flags = "h:w:f:o:";
    int c;

    struct shad_opts opts;
    opts.canvas_height = 0;
    opts.canvas_width = 0;
    if (argc == 2) {
        opts.filename = argv[1];
        return opts;
    } else {
        while((c = getopt(argc, (char *const*)argv, flags)) != -1) {
            switch(c) {
            case 'h':
                opts.canvas_height = strtol(optarg, NULL, 10);
                break;
            case 'w':
                opts.canvas_width = strtol(optarg, NULL, 10);
                break;
            case 'f':
                opts.filename = optarg;
                break;
            case 'o':
                opts.filename = optarg;
                break;
            }
        }
        return opts;
    }
}
