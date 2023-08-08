#include "color_picker_panel.h"
#include "palette.h"
#include "selected_color_panel.h"

static void fill_color_picker(struct ncpanel *color_picker, const int rows, const int cols);
static void proccess_input_and_update(struct ncpanel *panel, const struct ncinput *input, void *input_data);

struct ncpanel *create_color_picker_panel(struct ncplane *parent, struct ncpanel *canvas, const int y, const int x, const int rows, const int cols)
{
    struct ncpanel_list *panel_list = NULL;
    struct ncpanel *color_picker = NULL;
    color_picker = ncpanel_create(parent, y, x, rows, cols);
    if (color_picker) {
        panel_list = ncpanel_create_list();
        if (!panel_list) {
            ncpanel_destroy(color_picker);
            return NULL;
        }
        ncpanel_bind_input_callback(color_picker, proccess_input_and_update, panel_list);
        ncpanel_create_box(color_picker, rows - 4, cols - 1, 0x100);
        fill_color_picker(color_picker, rows - 4, cols - 1);

        struct ncpanel *selected_color = create_selected_color_panel(ncpanel_get_plane(color_picker), 
                                                                     rows - 3, 0, 3, cols, 0x100);
        if (selected_color == NULL) {
            ncpanel_destroy_list(panel_list);
            ncpanel_destroy(color_picker);
            return NULL;
        } else {
            ncpanel_add_panel_to_list(panel_list, selected_color);
        }
        ncpanel_bind_observer_and_subject(selected_color, color_picker);
        ncpanel_bind_observer_and_subject(canvas, color_picker);
    }
    return color_picker;
}

static void proccess_input_and_update(struct ncpanel *panel, const struct ncinput *input, void *input_data)
{
    struct ncplane *color_picker_plane = ncpanel_get_plane(panel);
    struct ncpanel_list *panel_list = (struct ncpanel_list*)input_data;
    int y = input->y;
    int x = input->x;
    uint64_t channels = 0;
    if (ncplane_translate_abs(color_picker_plane, &y, &x) == true) {
        if (input->id == NCKEY_BUTTON1) {
            ncplane_at_yx(color_picker_plane, y, x, NULL, &channels);
            uint32_t color = ncchannels_fg_rgb(channels);
            ncpanel_notify_observers(panel, &color, NCPANEL_EVENT_CHANGED_COLOR);
        }
        struct ncpanel_node *first = ncpanel_list_begin(panel_list);
        while (first) {
            ncpanel_proccess_input_and_update(ncpanel_get_panel(first), input);
            first = ncpanel_list_next(first);
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
