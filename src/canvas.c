#include "canvas.h"
#include "choosen_color_panel.h"

static struct ncplane *canvas = NULL;

static void draw_character_on_canvas(const struct ncinput *input);
static void erase_character_on_canvas(const struct ncinput *input);
static bool locked = false;

struct ncplane *init_canvas_plane(struct notcurses *nc)
{
    const char *panel_name = "canvas";
    unsigned int terminal_rows = 0;
    unsigned int terminal_cols = 0;

    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));
    notcurses_term_dim_yx(nc, &terminal_rows, &terminal_cols);
    
    opts.y = 1;
    opts.x = 1;
    opts.rows = terminal_rows - 2;
    opts.cols = terminal_cols / 2 + terminal_cols / 4;
    opts.name = panel_name;  
    canvas = ncplane_create(notcurses_stdplane(nc), &opts);
    if (canvas == NULL)
        return NULL;
    else 
        return canvas;
}

void proccess_input_on_canvas(const struct ncinput *input, struct ncplane *canvas)
{
    /* Canvas is locked - we can't proccess input */
    if (locked == true)
        return;
    int y = input->y;
    int x = input->x;
    
    if (ncplane_translate_abs(canvas, &y, &x) == true) {
        switch(input->id) {
        case NCKEY_BUTTON1:
            draw_character_on_canvas(input);
            break;
        case NCKEY_BUTTON3:
            erase_character_on_canvas(input);
            break;
        }
    }
}

void lock_canvas(void)
{
    locked = true;
}

void unlock_canvas(void)
{
    locked = false;
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
