#include "selected_color_panel.h"
#include "palette.h"
#include "src/color.h"

static const wchar_t *selected_color_str = L"Current color:";

static void update(struct ncpanel *panel, void *update_data);

struct ncpanel *create_selected_color_panel(struct ncpanel *parent, const int y, const int x, const int rows, const int cols, const uint32_t mask)
{
    struct ncpanel *selected_color_panel = ncpanel_create(ncpanel_get_plane(parent), y, x, rows, cols);
    if (selected_color_panel) {
        ncpanel_bind_update_callback(selected_color_panel, update, NULL);
        struct ncplane *selected_color_plane = ncpanel_get_plane(selected_color_panel);
        uint32_t selected_color = 0; 
        ncpalette_get(get_palette(), 1, &selected_color);

        ncpanel_create_box(selected_color_panel, rows - 1, cols - 1, mask);
        ncplane_putwstr_yx(selected_color_plane, rows / 2, 1, selected_color_str);
        ncplane_set_fg_rgb(selected_color_plane, selected_color);
        ncplane_putwc_yx(selected_color_plane, rows / 2,  wcslen(selected_color_str) + 2, L'█');
    }
    return selected_color_panel; 
}

static void update(struct ncpanel *panel, void *update_data)
{
    if (ncpanel_is_notifyed(panel) == true) {
        enum NCPANEL_OBSERVER_EVENT event = ncpanel_get_event(panel);
        if (event == NCPANEL_EVENT_CHANGED_COLOR) {
            struct ncplane *panel_plane = ncpanel_get_plane(panel);
            ncplane_set_fg_rgb(panel_plane, get_current_color());
            ncplane_putwc_yx(panel_plane, ncplane_dim_y(panel_plane) / 2,  wcslen(selected_color_str) + 2, L'█');
        }
    }
}
