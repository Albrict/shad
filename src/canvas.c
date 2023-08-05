#include "canvas.h"
#include "libs/libattopng.h"
#include "static_choosen_color_panel.h"

#define RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))

static struct ncplane *canvas = NULL;

static void draw_character_on_canvas(const struct ncinput *input);
static void erase_character_on_canvas(const struct ncinput *input);

static bool locked = false;
static void save_plane();

struct ncplane *init_canvas_plane(struct notcurses *nc)
{
    unsigned int terminal_rows = 0;
    unsigned int terminal_cols = 0;

    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));
    notcurses_term_dim_yx(nc, &terminal_rows, &terminal_cols);
    
    opts.y = 1;
    opts.x = 1;
    opts.rows = terminal_rows - 2;
    opts.cols = terminal_cols / 2 + terminal_cols / 4;
    
    canvas = ncplane_create(notcurses_stdplane(nc), &opts);

//    if (canvas) {
//        for (unsigned int y = 0; y < ncplane_dim_y(canvas); ++y) {
//            for (unsigned int x = 0; x < ncplane_dim_x(canvas); ++x)
//                ncplane_putchar_yx(canvas, y, x,  ' ');
//        }
//    }
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
        case NCKEY_BUTTON2:
            save_plane();
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
    ncplane_set_bg_rgb(canvas, get_choosen_color());
    ncplane_putchar_yx(canvas, input->ypx + input->y, input->xpx + input->x, ' ');
}

static void erase_character_on_canvas(const struct ncinput *input)
{
    ncplane_erase_region(canvas, input->ypx + input->y, input->xpx + input->x, 1, 1);
}

static void save_plane()
{
    const int height = ncplane_dim_y(canvas);
    const int width = ncplane_dim_x(canvas);

    libattopng_t *png = libattopng_new(width, height, PNG_PALETTE);
    int x, y;

    png = libattopng_new(width, height, PNG_RGBA);

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            struct nccell pixel;
            ncplane_at_yx_cell(canvas, y, x, &pixel);
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
