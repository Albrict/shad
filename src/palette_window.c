#include "palette_window.h"
#include "shad_error.h"
#include "palette.h"
#include "util.h"
#include "color_picker_panel.h"

static struct ncplane *palette_window = NULL;
static bool is_open = false;

static void fill_window(struct ncplane *palette_window, const int rows, const int cols);
static void create_boundraies(struct ncplane *palette_window, const int rows, const int cols);

struct ncplane *create_palette_window(struct ncplane *parent, bool *is_opened)
{
    const char *error_create_plane_message = "Can't create palette window\n";
    const char *panel_name = "palette-window";

    /* If it's already allocate - return */
    if (is_open == true) {
        *is_opened = is_open;
        return palette_window;
    }


    const int rows = 10;
    unsigned int term_rows, term_cols;
    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));
    notcurses_term_dim_yx(ncplane_notcurses_const(parent), &term_rows, &term_cols);
    
    opts.rows = rows;
    opts.cols = get_amount_of_colors() / 5;
    opts.y = term_rows / 2 - rows / 2;
    opts.x = term_cols / 2 - opts.cols / 2;
    opts.name = panel_name;

    palette_window = ncplane_create(parent, &opts);
    if (palette_window == NULL)
        die_and_log(error_create_plane_message);
    ncplane_move_family_top(palette_window);

    create_box(palette_window, opts.rows - 1, opts.cols - 1, 0x100);
    
    is_open = true;
    return palette_window;
}

void proccess_input_on_palette_window(const struct ncinput *input, struct ncplane *palette_window)
{
    int y = input->y;
    int x = input->x;
    
    if (ncplane_translate_abs(palette_window, &y, &x) == true) {
        ;
    }
}

static void create_boundraies(struct ncplane *palette_window, const int rows, const int cols)
{
    struct nccell ul;
    struct nccell ur;
    struct nccell ll;
    struct nccell lr;
    struct nccell hline;
    struct nccell vline;
    
    nccell_init(&ul);
    nccell_init(&ur);
    nccell_init(&ll);
    nccell_init(&lr);
    nccell_init(&hline);
    nccell_init(&vline);

    nccell_load(palette_window, &ul, "╭");
    nccell_load(palette_window, &ur, "╮");
    nccell_load(palette_window, &ll, "╰");
    nccell_load(palette_window, &lr, "╯");
    nccell_load(palette_window, &hline, "─");
    nccell_load(palette_window, &vline, "│");

    ncplane_box(palette_window, &ul, &ur, &ll, &lr, &hline, &vline, rows, cols, 0x100); 
}

static void fill_window(struct ncplane *palette, const int rows, const int cols)
{
    uint32_t color;
    int color_id = 1;

    ncpalette_get(get_palette(), 0,  &color);
    for (int y = 1; y < rows; ++y) {
        for (int x = 1; x < cols; ++x) {
            ncplane_set_bg_rgb(palette, color);
            ncplane_putwc_yx(palette, y, x, L' ');
        }
    }
    
    for (int y = 1; y < rows; ++y) {
        for (int x = 1; x < cols; ++x) {
            if (color_id == get_amount_of_colors())
                break;
            ncpalette_get(get_palette(), color_id,  &color);
            ncplane_set_fg_rgb(palette, color);
            ncplane_putwc_yx(palette, y, x, L'▊');
            ++color_id;
        }
    }
}
