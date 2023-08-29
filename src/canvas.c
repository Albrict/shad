#include "canvas.h"
#include "libs/libattopng.h"
#include "palette.h"
#include "src/color.h"

#define RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))

static void save_plane(struct ncplane *canvas_panel_plane, const char *filename);
static void proccess_input_on_canvas(struct ncpanel *canvas_panel, const struct ncinput *input, void *filename);
static void update_canvas(struct ncpanel *panel, void *update_data);

struct ncpanel *create_canvas_panel(struct ncplane *parent, const char *filename, 
                                    const unsigned int rows, const unsigned int cols)
{
    struct ncpanel *canvas_panel = NULL; 
    const int y = 1;
    const int x = 0;
    canvas_panel = ncpanel_create(parent, y, x, rows, cols);
    if (canvas_panel) {
        ncpanel_bind_input_callback(canvas_panel, proccess_input_on_canvas, NULL);
        ncpanel_bind_update_callback(canvas_panel, update_canvas, (void*)filename);
    }
    return canvas_panel;
}

static void draw_character_on_canvas(struct ncplane *canvas_panel_plane, const int y, const int x, const uint32_t color)
{
    ncplane_set_bg_rgb(canvas_panel_plane, color); 
    ncplane_putchar_yx(canvas_panel_plane, y, x, ' ');
}

static void erase_character_on_canvas(struct ncplane *canvas_panel_plane, const int y, const int x)
{
    ncplane_erase_region(canvas_panel_plane, y, x, 1, 1);
}

static void save_plane(struct ncplane *canvas_panel_plane, const char *filename)
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

    libattopng_save(png, filename);
    libattopng_destroy(png);
}

static void proccess_input_on_canvas(struct ncpanel *canvas_panel, const struct ncinput *input, void *filename)
{
    struct ncplane *canvas_panel_plane = ncpanel_get_plane(canvas_panel);
    int y = input->y;
    int x = input->x;
    if (ncplane_translate_abs(canvas_panel_plane, &y, &x) == true) {
        switch(input->id) {
        case NCKEY_BUTTON1:
            draw_character_on_canvas(canvas_panel_plane, y, x, get_current_color());
            break;
        case NCKEY_BUTTON3:
            erase_character_on_canvas(canvas_panel_plane, y, x);
            break;
        }
    }
}

static void update_canvas(struct ncpanel *panel, void *update_data)
{
    if (ncpanel_is_notifyed(panel) == true) {
        enum NCPANEL_OBSERVER_EVENT event = ncpanel_get_event(panel);
        if (event == NCPANEL_EVENT_SAVE) {
            const char *filename = (const char*)update_data;
            save_plane(ncpanel_get_plane(panel), filename);
        }
    }
}
