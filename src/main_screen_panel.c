#include "main_screen_panel.h"
#include "canvas.h"
#include "instrument_panel.h"
#include "menu.h"

static void update(struct ncpanel *main_screen, void *update_data);

struct ncpanel *create_main_screen_panel(struct notcurses *nc, bool *running, const char *filename)
{
    const int y = 1;
    const int x = 1;

    unsigned int terminal_rows, terminal_cols;
    notcurses_term_dim_yx(nc, &terminal_rows, &terminal_cols);
    
    struct ncpanel *main_screen = ncpanel_create(notcurses_stdplane(nc), y, x, terminal_rows, terminal_cols);
    struct ncpanel *canvas = NULL;
    struct ncpanel *instrument_panel = NULL;
    struct ncpanel *menu = NULL;
    if (main_screen) {
        const unsigned int canvas_rows = terminal_rows - 2;
        const unsigned int canvas_cols = terminal_cols / 2 + terminal_cols / 4;

        canvas = create_canvas_panel(notcurses_stdplane(nc), filename, canvas_rows, canvas_cols);
        if (!canvas) {
            goto ncpanel_err;
        } else {
            if (ncpanel_add_child(main_screen, canvas) == false)
                goto canvas_err;
        }

        instrument_panel = create_instrument_panel(notcurses_stdplane(nc), canvas);
        if (!instrument_panel) {
            goto ncpanel_err;
        } else {
            if (ncpanel_add_child(main_screen, instrument_panel) == false)
                goto instrument_panel_err;
        }
        
        menu = create_menu_panel(notcurses_stdplane(nc));
        if (!menu) {
            goto ncpanel_err;
        } else {
            if (ncpanel_add_child(main_screen, menu) == false)
                goto menu_err;
        }

        ncpanel_bind_update_callback(main_screen, update, running);
        ncpanel_bind_observer_and_subject(main_screen, menu);
        ncpanel_bind_observer_and_subject(canvas, menu);

        if (filename) {
            if (ncpanel_blit_image_from_file(filename, canvas, NCBLIT_1x1) == false)
                goto menu_err;
        }
    }
    return main_screen;
menu_err:
    ncpanel_destroy(menu);
instrument_panel_err:
    ncpanel_destroy(instrument_panel);
canvas_err:
    ncpanel_destroy(canvas);
    ncpanel_destroy(main_screen);
    return NULL;
ncpanel_err:
    ncpanel_destroy(main_screen);
    return NULL;
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
