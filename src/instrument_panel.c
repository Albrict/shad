#include "instrument_panel.h"
#include "shad_error.h"
#include "canvas.h"
#include "util.h"
#include "button.h"
#include "color_picker_panel.h"
#include "static_choosen_color_panel.h"
#include "palette_window.h"
#include "panel_list.h"

static struct button *more_colors_button = NULL;
static struct panel_list *list = NULL;

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
    lock_canvas();
}

struct ncplane *init_instrument_panel_plane(struct notcurses *nc)
{
    const wchar_t *button_text = L"Palette";
    const char *panel_name = "instruments";

    struct ncplane *instrument_panel = NULL;
    unsigned int terminal_rows = 0;
    unsigned int terminal_cols = 0;

    int color_picker_rows = 0;
    int color_picker_cols = 0;

    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));
    notcurses_term_dim_yx(nc, &terminal_rows, &terminal_cols);

    opts.y = 0;
    opts.x = terminal_cols / 2 + terminal_cols / 4 + 1;
    opts.rows = terminal_rows;
    opts.cols = terminal_cols - opts.x;
    opts.name = panel_name;
    instrument_panel = ncplane_create(notcurses_stdplane(nc), &opts);
    if (instrument_panel == NULL)
        return NULL;
    create_box(instrument_panel, opts.rows - 1, opts.cols - 1, 0x100);
    
    int color_picker_x = 1; 
    int color_picker_y = 1; 
    color_picker_cols = opts.cols - 2; 
    color_picker_rows = opts.rows / 4;
    
    list = create_list();
      
    struct ncplane *color_picker = create_color_picker_panel(instrument_panel, color_picker_y, color_picker_x, color_picker_rows, color_picker_cols);
    if (color_picker == NULL)
        return NULL;
    else
        add_panel_to_list(list, color_picker, proccess_input_on_color_picker);
    
    struct ncplane *choosen_color = create_static_choosen_color_panel(instrument_panel, color_picker_rows, 1, 3, color_picker_cols / 2, NCBOXMASK_TOP);
    if (choosen_color == NULL)
        return NULL;
    else
        add_panel_to_list(list, choosen_color, NULL);
    more_colors_button = create_button(instrument_panel, color_picker_rows, color_picker_cols / 2 + 1, 3, color_picker_cols / 2);
    draw_button_box(more_colors_button, NCBOXMASK_TOP);
    draw_text_on_button(more_colors_button, button_text);
    bind_callback(more_colors_button, callback_func, notcurses_stdplane(nc));
    return instrument_panel;
}

void close_instument_panel(void)
{
    destroy_list(list);
}

void proccess_input_on_instrument_panel_plane(const struct ncinput *input, struct ncplane *instrument_panel)
{
    int y = input->y;
    int x = input->x;
    
    if (ncplane_translate_abs(instrument_panel, &y, &x) == true) {
        if (input->id == NCKEY_BUTTON1) {
            struct panel_node *first = panel_list_begin(list);
            while (first) {
                panel_input_callback callback = get_panel_callback(first);
                if (callback)
                    callback(input, get_panel(first));
                first = panel_list_next(first);
            }
        }
    }
}
