#include "instrument_panel.h"
#include "shad_error.h"
#include "palette.h"
#include "util.h"
#include "button.h"

#include <notcurses/notcurses.h>

static struct ncplane *instrument_panel = NULL;
static struct ncplane *color_picker = NULL;
static uint32_t current_color = 0;

static void init_color_picker(const int y, const int x, const int rows, const int cols);
static void proccess_input_on_color_picker(const struct ncinput *input);

void init_instrument_panel_plane(struct notcurses *nc)
{
    unsigned int terminal_rows = 0;
    unsigned int terminal_cols = 0;

    int color_picker_rows = 0;
    int color_picker_cols = 0;

    const char *error_create_plane_message = "Can't create instrument panel plane\n";

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

    init_color_picker(color_picker_y, color_picker_x, color_picker_rows, color_picker_cols);
    ncpalette_get(get_palette(), 1, &current_color);

    struct button *button = create_button(instrument_panel, color_picker_rows, 1, 2, color_picker_cols / 2); 
    draw_box(button, NCBOXMASK_TOP);
}

const uint32_t get_current_color(void)
{
    return current_color;
}

void proccess_input_on_instrument_panel_plane(const struct ncinput *input)
{
    int y = input->y;
    int x = input->x;

    if (ncplane_translate_abs(color_picker, &y, &x) == true)
        if (input->id == NCKEY_BUTTON1)
            proccess_input_on_color_picker(input);
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

static void fill_color_picker(const int rows, const int cols)
{
    uint32_t color;
    int color_id = 1;
    for (int y = 1; y < rows; ++y) {
        for (int x = 1; x < cols; ++x) {
            if (color_id == 255)
                break;
            ncpalette_get(get_palette(), color_id,  &color);
            ncplane_set_fg_rgb(color_picker, color);
            ncplane_putwc_yx(color_picker, y, x, L'▊');
            ++color_id;
        }
    }
}

static void init_color_picker(const int y, const int x, const int rows, const int cols)
{
    const char *error_create_plane_message = "Can't create color picker panel plane\n";
    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));
    opts.y = y;
    opts.x = x;
    opts.rows = rows;
    opts.cols = cols;

    color_picker = ncplane_create(instrument_panel, &opts);
    if (color_picker == NULL)
        die_and_log(error_create_plane_message);

    create_box(color_picker, rows - 1, cols - 1, 0x100);
    fill_color_picker(rows - 1, cols - 1);
}

static void proccess_input_on_color_picker(const struct ncinput *input)
{
    int y = input->y;
    int x = input->x;
    uint64_t channels = 0;
    
    ncplane_translate_abs(color_picker, &y, &x);
    ncplane_at_yx(color_picker, y, x, NULL, &channels);
    current_color = ncchannels_fg_rgb(channels); 
}
