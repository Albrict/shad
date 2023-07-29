#include "palette_window.h"
#include "shad_error.h"
#include "util.h"

static struct ncplane *palette_window = NULL;

void create_palette_window(struct ncplane *parent)
{
    const char *error_create_plane_message = "Can't create palette window\n";
    int parent_y, parent_x;
    struct ncplane_options opts;
    memset(&opts, 0, sizeof(struct ncplane_options));
    ncplane_yx(parent, &parent_y, &parent_x);
    
    opts.x = 10;
    opts.y = 5;
    opts.cols = 20;
    opts.rows = 20;
      
    palette_window = ncplane_create(parent, &opts);
    if (palette_window == NULL)
        die_and_log(error_create_plane_message);

    create_box(palette_window, opts.rows - 1, opts.cols - 1, 0x100);
    ncplane_move_family_top(palette_window);
}

void destroy_palette_window(void)
{
    ;
}
