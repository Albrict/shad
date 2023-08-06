#include "instrument_panel.h"
#include "shad_error.h"
#include "canvas.h"
#include "util.h"
#include "button.h"
#include "color_picker_panel.h"
#include "selected_color_panel.h"
#include "palette_window.h"

//static struct button *more_colors_button = NULL;
//static struct panel_list *list = NULL;

void callback_func(void *data)
{
//    const char *error_palette_window_init_message = "Can't create palette window!\n";
//    bool is_opened = false;
//    struct ncplane *parent = (struct ncplane*)data;
//    struct ncplane *palette_window = create_palette_window(parent, &is_opened);
//    if (palette_window == NULL)
//        die_and_log(error_palette_window_init_message);
//
//    /* Window is opened - no need to add it to the list */
//    if (is_opened == true)
//        return;
//
    /* We're adding this window to global panel list */
//    add_panel_to_list(palette_window, proccess_input_on_palette_window);
//    lock_canvas();
}

static void proccess_input_on_instrument_panel(struct ncpanel *panel, const struct ncinput *input, void *data);
static void clean_up_callback(struct ncpanel *panel, void *clean_up_data);

struct ncpanel *create_instrument_panel(struct ncplane *parent)
{
    struct ncpanel *instrument_panel = NULL;
    struct ncpanel_list *panel_list = NULL;
    struct notcurses *nc = ncplane_notcurses(parent);

    unsigned int terminal_rows;
    unsigned int terminal_cols;
    notcurses_term_dim_yx(nc, &terminal_rows, &terminal_cols);

    const int y = 0;
    const int x = terminal_cols / 2 + terminal_cols / 4 + 1;
    const unsigned int rows = terminal_rows;
    const unsigned int cols = terminal_cols - x;

    instrument_panel = ncpanel_create(parent, y, x, rows, cols); 
    if (instrument_panel) {
        panel_list = ncpanel_create_list();
        if (panel_list == NULL)
            return NULL;

        ncpanel_create_box(instrument_panel, rows - 1, cols - 1, 0x100);
        ncpanel_bind_input_callback(instrument_panel, proccess_input_on_instrument_panel, panel_list);
        ncpanel_bind_clean_up_callback(instrument_panel, clean_up_callback, panel_list);

        int color_picker_x = 1; 
        int color_picker_y = 1; 
        int color_picker_cols = cols - 2; 
        int color_picker_rows = rows / 4;

        struct ncpanel *color_picker = create_color_picker_panel(ncpanel_get_plane(instrument_panel), color_picker_y, color_picker_x, 
                                                                 color_picker_rows, color_picker_cols);
        
        if (color_picker == NULL)
            return NULL;
        else
            ncpanel_add_panel_to_list(panel_list, color_picker);
//        
        struct ncpanel *selected_color = create_selected_color_panel(ncpanel_get_plane(instrument_panel), 
                                                                     color_picker_rows, 1, 3, color_picker_cols / 2, NCBOXMASK_TOP);
        if (selected_color == NULL)
            return NULL;
        else
            ncpanel_add_panel_to_list(panel_list, selected_color);
//        more_colors_button = create_button(instrument_panel, color_picker_rows, color_picker_cols / 2 + 1, 3, color_picker_cols / 2);
//        draw_button_box(more_colors_button, NCBOXMASK_TOP);
//        draw_text_on_button(more_colors_button, button_text);
//        bind_callback(more_colors_button, callback_func, notcurses_stdplane(nc));
    }
    
    return instrument_panel;
}

static void proccess_input_on_instrument_panel(struct ncpanel *panel, const struct ncinput *input, void *data)
{
    struct ncplane *instrument_panel_plane = ncpanel_get_plane(panel);
    struct ncpanel_list *panel_list = (struct ncpanel_list*)data;

    int y = input->y;
    int x = input->x;
    
    if (ncplane_translate_abs(instrument_panel_plane, &y, &x) == true) {
        struct ncpanel_node *first = ncpanel_list_begin(panel_list);
        while (first) {
            ncpanel_proccess_input(ncpanel_get_panel(first), input);
            first = ncpanel_list_next(first);
        }
    }
}

static void clean_up_callback(struct ncpanel *panel, void *clean_up_data)
{
    struct ncpanel_list *panel_list = (struct ncpanel_list*)clean_up_data;
    ncpanel_destroy_list(panel_list);
}
