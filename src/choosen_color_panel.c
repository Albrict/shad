#include "choosen_color_panel.h"
#include "palette.h"
#include "shad_error.h"
#include "util.h"

struct ncplane *choosen_color_panel = NULL;
uint32_t current_color = 0;

static const wchar_t *current_choosen_color = L"Current color:";

void init_choosen_color_panel(struct ncplane *parent, const int y, const int x, const int rows, const int cols, const uint32_t mask)
{
    const char *error_create_plane_message = "Can't create choosen color panel plane\n";
    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));
    opts.y = y;
    opts.x = x;
    opts.rows = rows;
    opts.cols = cols;

    choosen_color_panel = ncplane_create(parent, &opts);
    if (choosen_color_panel == NULL)
        die_and_log(error_create_plane_message);

    ncpalette_get(get_palette(), 1, &current_color);
    create_box(choosen_color_panel, rows - 1, cols - 1, mask);
    ncplane_putwstr_yx(choosen_color_panel, rows / 2, 1, current_choosen_color);
    ncplane_set_fg_rgb(choosen_color_panel, current_color);
    ncplane_putwc_yx(choosen_color_panel, rows / 2,  wcslen(current_choosen_color) + 2, L'█');
}

void set_color(const uint32_t new_color)
{
    current_color = new_color;
    ncplane_set_fg_rgb(choosen_color_panel, current_color);
    ncplane_putwc_yx(choosen_color_panel, ncplane_dim_y(choosen_color_panel) / 2,  wcslen(current_choosen_color) + 2, L'█');
}

const uint32_t get_choosen_color(void)
{
    return current_color;
}
