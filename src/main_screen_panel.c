#include "main_screen_panel.h"
#include "canvas.h"
#include "instrument_panel.h"
#include "menu.h"

static void proccess_input_and_update(struct ncpanel *main_window, const struct ncinput *input, void *data);
static void clean_up(struct ncpanel *main_window, void *clean_up_data);
static void update(struct ncpanel *main_screen, void *update_data);

struct ncpanel *create_main_screen_panel(struct notcurses *nc, bool *running, char *filename)
{
    const int y = 1;
    const int x = 1;

    unsigned int rows, cols;
    notcurses_term_dim_yx(nc, &rows, &cols);
    
    struct ncpanel *main_screen = ncpanel_create(notcurses_stdplane(nc), y, x, rows, cols);
    if (main_screen) {
        struct ncpanel_list *panel_list = ncpanel_create_list();
        if (!panel_list)
            return NULL;

        struct ncpanel *canvas = create_canvas_panel(notcurses_stdplane(nc), filename);
        if (!canvas)
            return NULL;
        else
            ncpanel_add_panel_to_list(panel_list, canvas);
        

        struct ncpanel *instrument_panel = create_instrument_panel(notcurses_stdplane(nc), canvas);
        if (!instrument_panel)
            return NULL;
        else
            ncpanel_add_panel_to_list(panel_list, instrument_panel);
        
        struct ncpanel *menu = create_menu_panel(notcurses_stdplane(nc));
        if (!menu)
            return NULL;
        else
            ncpanel_add_panel_to_list(panel_list, menu);

        ncpanel_bind_input_callback(main_screen, proccess_input_and_update, panel_list);
        ncpanel_bind_update_callback(main_screen, update, running);
        ncpanel_bind_clean_up_callback(main_screen, clean_up, panel_list);

        ncpanel_bind_observer_and_subject(main_screen, menu);
        ncpanel_bind_observer_and_subject(canvas, menu);
    }
    return main_screen;
}

static void proccess_input_and_update(struct ncpanel *main_window, const struct ncinput *input, void *data)
{
    struct ncpanel_list *panel_list = (struct ncpanel_list*)data;
    struct ncpanel_node *first = ncpanel_list_begin(panel_list);
    while (first) {
        ncpanel_proccess_input_and_update(ncpanel_get_panel(first), input);
        first = ncpanel_list_next(first);
    }
}

static void clean_up(struct ncpanel *main_window, void *clean_up_data)
{
    struct ncpanel_list *panel_list = (struct ncpanel_list*)clean_up_data;
    ncpanel_destroy_list(panel_list);
    panel_list = NULL;
}

static void update(struct ncpanel *main_screen, void *update_data)
{
    if (ncpanel_is_notifyed(main_screen) == true) {
        enum NCPANEL_OBSERVER_EVENT event = ncpanel_get_event(main_screen);
        if (event == NCPANEL_EVENT_EXIT) {
            bool *running = (bool*)update_data;
            *running = false;
        }
    }
}
