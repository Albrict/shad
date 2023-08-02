#include "color_picker_panel.h"
#include "static_choosen_color_panel.h"
#include "palette.h"
#include "util.h"

static void fill_color_picker(struct ncplane *color_picker, const int rows, const int cols);

struct ncplane *create_color_picker_panel(struct ncplane *parent, const int y, const int x, const int rows, const int cols)
{
    struct ncplane *color_picker = NULL;
    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));
    opts.y = y;
    opts.x = x;
    opts.rows = rows;
    opts.cols = cols;

    color_picker = ncplane_create(parent, &opts);
    if (color_picker == NULL)
        return NULL;

    create_box(color_picker, rows - 1, cols - 1, 0x100);
    fill_color_picker(color_picker, rows - 1, cols - 1);
    return color_picker;
}

void proccess_input_on_color_picker(const struct ncinput *input, struct ncplane *color_picker)
{
    int y = input->y;
    int x = input->x;
    uint64_t channels = 0;
    if (ncplane_translate_abs(color_picker, &y, &x) == true) {
        ncplane_at_yx(color_picker, y, x, NULL, &channels);
        set_color(ncchannels_fg_rgb(channels));
    } else {
        return;
    }
}

static void fill_color_picker(struct ncplane *color_picker, const int rows, const int cols)
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
