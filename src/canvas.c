#include "canvas.h"
#include "libs/libattopng.h"
#include "src/shad_error.h"
#include "static_choosen_color_panel.h"

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

    if (canvas) {
        /* Fill up plane with white color */
        ncplane_set_fg_rgb(canvas, 0xFFFFFF);
        for (unsigned int y = 0; y < ncplane_dim_y(canvas); ++y) {
            for (unsigned int x = 0; x < ncplane_dim_x(canvas); ++x)
                ncplane_putwc_yx(canvas, y, x,  L'█');
        }
    }
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
    ncplane_set_fg_rgb(canvas, get_choosen_color());
    ncplane_putwc_yx(canvas, input->ypx + input->y, input->xpx + input->x, L'█');
}

static void erase_character_on_canvas(const struct ncinput *input)
{
    ncplane_set_fg_rgb(canvas, 0xFFFFFF);
    ncplane_putwc_yx(canvas, input->ypx + input->y, input->xpx + input->x, L'█');
}

static void save_plane()
{
    const int height = ncplane_dim_y(canvas);
    const int width = ncplane_dim_x(canvas);

    uint32_t *data = ncplane_as_rgba(canvas, NCBLIT_1x1, 0, 0, ncplane_dim_y(canvas), ncplane_dim_x(canvas), NULL, NULL);
    if (data == NULL)
        die_and_log("Can't save");
    libattopng_t *png = libattopng_new(width, height, PNG_PALETTE);
    int x, y;

    png = libattopng_new(width, height, PNG_RGBA);
    
    size_t pixel = 0;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            libattopng_set_pixel(png, x, y, data[pixel]);
            ++pixel;
        }
    }

    libattopng_save(png, "test.png");
    libattopng_destroy(png);
}
