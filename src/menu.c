#include "menu.h"

static void proccess_input_on_menu(struct ncpanel *panel, const struct ncinput *input, void *data);

struct ncpanel *create_menu_panel(struct ncplane *parent)
{
    struct ncmenu_item file_items[] = {
        { .desc = "Save", .shortcut = {.id = 's' } },
        { .desc = NULL },
        { .desc = "Quit", .shortcut = { .id = 'q' } }
    };

    struct ncmenu_section sections[] = {
    { .name = "File", .items = file_items,
      .itemcount = sizeof(file_items) / sizeof(*file_items),
      .shortcut = { .id = 'f', }, },
    };

    ncmenu_options mopts;
    memset(&mopts, 0, sizeof(mopts));
    mopts.sections = sections;
    mopts.sectioncount = sizeof(sections) / sizeof(*sections);
    ncchannels_set_fg_rgb(&mopts.headerchannels, 0x00ff00);
    ncchannels_set_bg_rgb(&mopts.headerchannels, 0x000000);
    ncchannels_set_fg_rgb(&mopts.sectionchannels, 0xb0d700);
    ncchannels_set_bg_rgb(&mopts.sectionchannels, 0x000000);

    unsigned rows, cols;
    struct notcurses *nc = ncplane_notcurses(parent);
    struct ncplane *plane = notcurses_stddim_yx(nc, &rows, &cols);
    struct ncmenu *menu = ncmenu_create(plane, &mopts);
    struct ncpanel *panel = NULL;
    if (menu) {
        panel = ncpanel_wrap_plane(ncmenu_plane(menu), 0, 0, rows, cols);
        if (panel)
            ncpanel_bind_input_callback(panel, proccess_input_on_menu, menu);
    }
    return panel;
}

static void proccess_input_on_menu(struct ncpanel *panel, const struct ncinput *input, void *data)
{
    struct ncmenu *menu = (struct ncmenu*)data; 

    if(!ncmenu_offer_input(menu, input)){
        const char* sel;
        if( (sel = ncmenu_mouse_selected(menu, input, NULL)) ){
            if(sel && !strcmp(sel, "Quit"))
                ncpanel_notify_observers(panel, NULL, NCPANEL_EVENT_EXIT);
            if(sel && !strcmp(sel, "Save"))
                ncpanel_notify_observers(panel, NULL, NCPANEL_EVENT_SAVE);
            if (input->id == 'q')
                ncpanel_notify_observers(panel, NULL, NCPANEL_EVENT_EXIT);
        }
    }
}
