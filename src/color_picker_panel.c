#include "color_picker_panel.h"
#include "shad_error.h"
#include "palette.h"
#include "util.h"

struct ncplane *color_picker = NULL;

static void fill_color_picker(const int rows, const int cols);

void init_color_picker(struct ncplane *parent, const int y, const int x, const int rows, const int cols)
{
    const char *error_create_plane_message = "Can't create color picker panel plane\n";
    const char *panel_name = "color-picker";

    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));
    opts.y = y;
    opts.x = x;
    opts.rows = rows;
    opts.cols = cols;
    opts.name = panel_name;

    color_picker = ncplane_create(parent, &opts);
    if (color_picker == NULL)
        die_and_log(error_create_plane_message);

    create_box(color_picker, rows - 1, cols - 1, 0x100);
    fill_color_picker(rows - 1, cols - 1);
}

uint32_t get_selected_color(const struct ncinput *input)
{
    int y = input->y;
    int x = input->x;
    uint64_t channels = 0;
    ncplane_translate_abs(color_picker, &y, &x);
    ncplane_at_yx(color_picker, y, x, NULL, &channels);
    return ncchannels_fg_rgb(channels); 
}

bool is_mouse_on_color_picker(const int y, const int x)
{
    int mouse_y = y;
    int mouse_x = x;

    return ncplane_translate_abs(color_picker, &mouse_y, &mouse_x);
}

static void fill_color_picker(const int rows, const int cols)
{
    uint32_t color;
    int color_id = 1;
    for (int y = 1; y < rows; ++y) {
        for (int x = 1; x < cols; ++x) {
            if (color_id == get_amount_of_colors())
                break;
            ncpalette_get(get_palette(), color_id,  &color);
            ncplane_set_fg_rgb(color_picker, color);
            ncplane_putwc_yx(color_picker, y, x, L'▊');
            ++color_id;
        }
    }
}
