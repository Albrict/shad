#include "instrument_panel.h"
#include "shad_error.h"
#include "util.h"
#include "button.h"
#include "color_picker_panel.h"
#include "choosen_color_panel.h"

static struct ncplane *instrument_panel = NULL;
static struct button *more_colors_button = NULL;

void init_instrument_panel_plane(struct notcurses *nc)
{
    unsigned int terminal_rows = 0;
    unsigned int terminal_cols = 0;

    int color_picker_rows = 0;
    int color_picker_cols = 0;

    const char *error_create_plane_message = "Can't create instrument panel plane\n";
    const wchar_t *button_text = L"Palette";
    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));
    notcurses_term_dim_yx(nc, &terminal_rows, &terminal_cols);

    opts.y = 0;
    opts.x = terminal_cols / 2 + terminal_cols / 4 + 1;
    opts.rows = terminal_rows;
    opts.cols = terminal_cols - opts.x;

    instrument_panel = ncplane_create(notcurses_stdplane(nc), &opts);
    if (instrument_panel == NULL)
        die_and_log(error_create_plane_message);
    create_box(instrument_panel, opts.rows - 1, opts.cols - 1, 0x100);

    int color_picker_x = 1; 
    int color_picker_y = 1; 
    color_picker_cols = opts.cols - 2; 
    color_picker_rows = opts.rows / 4;

    init_color_picker(instrument_panel, color_picker_y, color_picker_x, color_picker_rows, color_picker_cols);
    init_choosen_color_panel(instrument_panel, color_picker_rows, 1, 3, color_picker_cols / 2, NCBOXMASK_TOP);

    more_colors_button = create_button(instrument_panel, color_picker_rows, color_picker_cols / 2 + 1, 3, color_picker_cols / 2);
    draw_button_box(more_colors_button, NCBOXMASK_TOP);
    draw_text_on_button(more_colors_button, button_text);
}

void proccess_input_on_instrument_panel_plane(const struct ncinput *input)
{
    int y = input->y;
    int x = input->x;
    
    proccess_input_on_button(more_colors_button, input);
    if (input->id == NCKEY_BUTTON1)
        if (is_mouse_on_color_picker(y, x) == true)
            set_color(get_selected_color(input));
}

struct ncplane *get_instument_panel_plane(void)
{
    return instrument_panel;
}

const struct ncplane *get_const_instument_panel_plane(void)
{
    const struct ncplane *const_instrument_panel = instrument_panel;
    return const_instrument_panel;
}
