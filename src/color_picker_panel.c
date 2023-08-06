#include "color_picker_panel.h"
#include "palette.h"
#include "state.h"

static void fill_color_picker(struct ncpanel *color_picker, const int rows, const int cols);

static void proccess_input_on_color_picker(struct ncpanel *panel, const struct ncinput *input, void *input_data);

struct ncpanel *create_color_picker_panel(struct ncplane *parent, const int y, const int x, const int rows, const int cols)
{
    struct ncpanel *color_picker = NULL;
    color_picker = ncpanel_create(parent, y, x, rows, cols);
    if (color_picker) {
        ncpanel_bind_input_callback(color_picker, proccess_input_on_color_picker, NULL);
        ncpanel_create_box(color_picker, rows - 1, cols - 1, 0x100);
        fill_color_picker(color_picker, rows - 1, cols - 1);
    }

    return color_picker;
}

static void proccess_input_on_color_picker(struct ncpanel *panel, const struct ncinput *input, void *input_data)
{
    struct ncplane *color_picker_plane = ncpanel_get_plane(panel);
    int y = input->y;
    int x = input->x;
    uint64_t channels = 0;
    if (ncplane_translate_abs(color_picker_plane, &y, &x) == true) {
        if (input->id == NCKEY_BUTTON1) {
            ncplane_at_yx(color_picker_plane, y, x, NULL, &channels);
            select_color(ncchannels_fg_rgb(channels));
        }
    } 
}

static void fill_color_picker(struct ncpanel *color_picker, const int rows, const int cols)
{
    struct ncplane *color_picker_plane = ncpanel_get_plane(color_picker);
    uint32_t color;
    int color_id = 1;
    for (int y = 1; y < rows; ++y) {
        for (int x = 1; x < cols; ++x) {
            if (color_id == get_amount_of_colors())
                break;
            ncpalette_get(get_palette(), color_id,  &color);
            ncplane_set_fg_rgb(color_picker_plane, color);
            ncplane_putwc_yx(color_picker_plane, y, x, L'▊');
            ++color_id;
        }
    }
}
