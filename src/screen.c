#include "screen.h"
#include "shad_error.h"
#include "color.h"

#include <notcurses/notcurses.h>

static struct ncplane *canvas = NULL;
static struct ncplane *instrument_panel = NULL;

static unsigned int terminal_rows = 0;
static unsigned int terminal_cols = 0;

inline static void set_color(const uint32_t new_color);

void init_canvas_plane(struct notcurses *nc)
{
    const char *error_create_plane_message = "Can't create canvas plane\n";

    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));
    notcurses_term_dim_yx(nc, &terminal_rows, &terminal_cols);
    
    opts.y = 1;
    opts.x = 1;
    opts.rows = terminal_rows;
    opts.cols = terminal_cols / 2 + terminal_cols / 4;
    
    canvas = ncplane_create(notcurses_stdplane(nc), &opts);
    if (canvas == NULL)
        die_and_log(error_create_plane_message);
}

struct ncplane *get_canvas_plane(void)
{
    return canvas;
}

const struct ncplane *get_const_canvas_plane(void)
{
    const struct ncplane *const_canvas = canvas;
    return const_canvas;
}

void init_instrument_panel_plane(struct notcurses *nc)
{
    const char *error_create_plane_message = "Can't create instrument panel plane\n";

    /* nccells for creating box */
    struct nccell ul;
    struct nccell ur;
    struct nccell ll;
    struct nccell lr;
    struct nccell hline;
    struct nccell vline;
    
    nccell_init(&ul);
    nccell_init(&ur);
    nccell_init(&ll);
    nccell_init(&lr);
    nccell_init(&hline);
    nccell_init(&vline);

//    nccells_load_box(canvas, NCSTYLE_NONE, 0, &ul, &ur, &ll, &lr, &hline, &vline, gcluster);
    nccell_load(instrument_panel, &ul, "╭");
    nccell_load(instrument_panel, &ur, "╮");
    nccell_load(instrument_panel, &ll, "╰");
    nccell_load(instrument_panel, &lr, "╯");
    nccell_load(instrument_panel, &hline, "─");
    nccell_load(instrument_panel, &vline, "│");

    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));

    opts.y = 0;
    opts.x = terminal_cols / 2 + terminal_cols / 4 + 1;
    opts.rows = terminal_rows;
    opts.cols = terminal_cols - opts.x;
    
    instrument_panel = ncplane_create(notcurses_stdplane(nc), &opts);
    if (instrument_panel == NULL)
        die_and_log(error_create_plane_message);

    ncplane_box(instrument_panel, &ul, &ur, &ll, &lr, &hline, &vline, opts.rows - 1, opts.cols - 1, 0x100); 
    ncplane_putchar(instrument_panel, 'c');  
}

void draw_character_on_canvas(const struct ncinput *input)
{
    set_color(RED);
    ncplane_putwc_yx(canvas, input->ypx + input->y, input->xpx + input->x, L'█');
}

void erase_character_on_canvas(const struct ncinput *input)
{
    ncplane_putchar_yx(get_canvas_plane(), input->ypx + input->y, input->xpx + input->x, ' ');
}

inline static void set_color(const uint32_t color)
{
    ncplane_set_fg_rgb(canvas, color);
}
