#include "canvas.h"
#include "libs/libattopng.h"
#include "state.h"

#define RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))


static bool locked = false;
static void save_plane(struct ncplane *canvas_panel_plane);

static void proccess_input_on_canvas(struct ncpanel *canvas_panel, const struct ncinput *input, void *data);

struct ncpanel *create_canvas_panel(struct ncplane *parent)
{
    struct ncpanel *canvas_panel = NULL; 
    struct notcurses *nc = ncplane_notcurses(parent);

    unsigned int terminal_rows;
    unsigned int terminal_cols;
    notcurses_term_dim_yx(nc, &terminal_rows, &terminal_cols);
    
    const int y = 1;
    const int x = 1;
    unsigned int rows = terminal_rows - 2;
    unsigned int cols = terminal_cols / 2 + terminal_cols / 4;
    
    canvas_panel = ncpanel_create(parent, y, x, rows, cols);
    if (canvas_panel)
        ncpanel_bind_input_callback(canvas_panel, proccess_input_on_canvas, NULL);
    return canvas_panel;
}

static void draw_character_on_canvas(struct ncplane *canvas_panel_plane, const struct ncinput *input)
{
    ncplane_set_bg_rgb(canvas_panel_plane, get_selected_color());
    ncplane_putchar_yx(canvas_panel_plane, input->ypx + input->y, input->xpx + input->x, ' ');
}

static void erase_character_on_canvas(struct ncplane *canvas_panel_plane, const struct ncinput *input)
{
    ncplane_erase_region(canvas_panel_plane, input->ypx + input->y, input->xpx + input->x, 1, 1);
}

static void save_plane(struct ncplane *canvas_panel_plane)
{
    const int height = ncplane_dim_y(canvas_panel_plane);
    const int width = ncplane_dim_x(canvas_panel_plane);

    libattopng_t *png = libattopng_new(width, height, PNG_PALETTE);
    int x, y;

    png = libattopng_new(width, height, PNG_RGBA);

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            struct nccell pixel;
            ncplane_at_yx_cell(canvas_panel_plane, y, x, &pixel);
            if (pixel.width == 0)
                libattopng_set_pixel(png, x, y, RGBA(0, 0, 0, 0));
            else {
                unsigned int r, g, b;
                nccell_bg_rgb8(&pixel, &r, &g, &b);
                libattopng_set_pixel(png, x, y, RGBA(r, g, b, 255));
            }
        }
    }

    libattopng_save(png, "test.png");
    libattopng_destroy(png);
}

static void proccess_input_on_canvas(struct ncpanel *canvas_panel, const struct ncinput *input, void *data)
{
    /* Canvas is locked - we can't proccess input */
    if (locked == true)
        return;
    struct ncplane *canvas_panel_plane = ncpanel_get_plane(canvas_panel);
    int y = input->y;
    int x = input->x;
    
    if (ncplane_translate_abs(canvas_panel_plane, &y, &x) == true) {
        switch(input->id) {
        case NCKEY_BUTTON1:
            draw_character_on_canvas(canvas_panel_plane, input);
            break;
        case NCKEY_BUTTON3:
            erase_character_on_canvas(canvas_panel_plane, input);
            break;
        case NCKEY_BUTTON2:
            save_plane(canvas_panel_plane);
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
