#include "selected_color_panel.h"
#include "palette.h"
#include "state.h"

static const wchar_t *selected_color_str = L"Selected color:";

static void proccess_input_on_selected_color(struct ncpanel *panel, const struct ncinput *input, void *input_data);

struct ncpanel *create_selected_color_panel(struct ncplane *parent, const int y, const int x, const int rows, const int cols, const uint32_t mask)
{
    struct ncpanel *selected_color_panel = ncpanel_create(parent, y, x, rows, cols);
    if (selected_color_panel) {
        ncpanel_bind_input_callback(selected_color_panel, proccess_input_on_selected_color, NULL);
        struct ncplane *selected_color_plane = ncpanel_get_plane(selected_color_panel);
        uint32_t selected_color = get_selected_color();
        ncpalette_get(get_palette(), 1, &selected_color);

        ncpanel_create_box(selected_color_panel, rows - 1, cols - 1, mask);
        ncplane_putwstr_yx(selected_color_plane, rows / 2, 1, selected_color_str);
        ncplane_putwc_yx(selected_color_plane, rows / 2,  wcslen(selected_color_str) + 2, L'█');
    }
    return selected_color_panel; 
}

static void proccess_input_on_selected_color(struct ncpanel *panel, const struct ncinput *input, void *input_data)
{
    struct ncplane *selected_color_plane = ncpanel_get_plane(panel);
    const uint32_t current_color = get_selected_color();
    ncplane_set_fg_rgb(selected_color_plane, current_color);
    ncplane_putwc_yx(selected_color_plane, ncplane_dim_y(selected_color_plane) / 2,  wcslen(selected_color_str) + 2, L'█');
}
