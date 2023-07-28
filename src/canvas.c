#include "canvas.h"
#include "choosen_color_panel.h"
#include "shad_error.h"

static struct ncplane *canvas = NULL;

static void draw_character_on_canvas(const struct ncinput *input);
static void erase_character_on_canvas(const struct ncinput *input);

void init_canvas_plane(struct notcurses *nc)
{
    unsigned int terminal_rows = 0;
    unsigned int terminal_cols = 0;
    const char *error_create_plane_message = "Can't create canvas plane\n";

    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));
    notcurses_term_dim_yx(nc, &terminal_rows, &terminal_cols);
    
    opts.y = 1;
    opts.x = 1;
    opts.rows = terminal_rows - 2;
    opts.cols = terminal_cols / 2 + terminal_cols / 4;
    
    canvas = ncplane_create(notcurses_stdplane(nc), &opts);
    if (canvas == NULL)
        die_and_log(error_create_plane_message);
}

void proccess_input_on_canvas(const struct ncinput *input)
{
    switch(input->id) {
    case NCKEY_BUTTON1:
        draw_character_on_canvas(input);
        break;
    case NCKEY_BUTTON3:
        erase_character_on_canvas(input);
        break;
    }
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

static void draw_character_on_canvas(const struct ncinput *input)
{
    ncplane_set_fg_rgb(canvas, get_choosen_color());
    ncplane_putwc_yx(canvas, input->ypx + input->y, input->xpx + input->x, L'█');
}

static void erase_character_on_canvas(const struct ncinput *input)
{
    ncplane_putchar_yx(canvas, input->ypx + input->y, input->xpx + input->x, ' ');
}
