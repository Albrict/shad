#include "instrument_panel.h"
#include "color_picker_panel.h"
#include "selected_color_panel.h"

struct ncpanel *create_instrument_panel(struct ncplane *parent, struct ncpanel *canvas)
{
    struct notcurses *nc = ncplane_notcurses(parent);
    unsigned int terminal_rows;
    unsigned int terminal_cols;
    notcurses_term_dim_yx(nc, &terminal_rows, &terminal_cols);

    const int y = 1;
    const int x = terminal_cols / 2 + terminal_cols / 4 + 1;
    const unsigned int rows = terminal_rows - 1;
    const unsigned int cols = terminal_cols - x;

    struct ncpanel *instrument_panel = ncpanel_create(parent, y, x, rows, cols); 
    struct ncpanel *color_picker = NULL;
    struct ncpanel *selected_color = NULL;

    if (instrument_panel) {
        ncpanel_create_box(instrument_panel, rows - 1, cols - 1, 0x100);

        const int color_picker_x = 1; 
        const int color_picker_y = 1; 
        const int color_picker_cols = cols - 2; 
        const int color_picker_rows = (rows / 4) + 3;

        color_picker = create_color_picker_panel(instrument_panel, color_picker_y, color_picker_x,  color_picker_rows, color_picker_cols);
        if (!color_picker) {
            goto ncpanel_err;
        } else {
            if (ncpanel_add_child(instrument_panel, color_picker) == false)
                goto color_picker_err;
        }

        selected_color = create_selected_color_panel(color_picker, color_picker_rows - 3, 0, 3, color_picker_cols, 0x100);
        if (!selected_color) {
            goto ncpanel_err;
        } else {
            if (ncpanel_add_child(instrument_panel, selected_color) == false)
                goto selected_color_err;
        }
        
        if (ncpanel_bind_observer_and_subject(selected_color, color_picker) == false)
            goto ncpanel_err;
    }
    return instrument_panel;

selected_color_err:
    ncpanel_destroy(selected_color);
color_picker_err:
    ncpanel_destroy(color_picker);
ncpanel_err:
    ncpanel_destroy(instrument_panel);
    return NULL;
}
