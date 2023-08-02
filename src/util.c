#include "util.h"

void create_box(struct ncplane *plane, const int rows, const int cols, unsigned int mask)
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

    nccell_load(plane, &ul, "╭");
    nccell_load(plane, &ur, "╮");
    nccell_load(plane, &ll, "╰");
    nccell_load(plane, &lr, "╯");
    nccell_load(plane, &hline, "─");
    nccell_load(plane, &vline, "│");

    ncplane_box(plane, &ul, &ur, &ll, &lr, &hline, &vline, rows, cols, mask); 
}

bool blit_image_on_plane(const char *filename, struct notcurses *nc, struct ncplane *plane)
{
    struct ncvisual_options opts; 
    memset(&opts, 0, sizeof(struct ncvisual_options));

    opts.n = plane;
    opts.x = 0;
    opts.y = 0;
    opts.begy = 0;
    opts.begx = 0;
    opts.lenx = ncplane_dim_x(plane);
    opts.leny = ncplane_dim_y(plane); 
    opts.blitter = NCBLIT_1x1;
    opts.flags = NCVISUAL_OPTION_ADDALPHA;
    opts.transcolor = 0xFF0000;
    opts.pxoffx = 0;
    opts.pxoffy = 0;

    struct ncvisual *visual = ncvisual_from_file(filename);
    if (visual) {
        ncvisual_resize(visual, opts.leny, opts.lenx);
        plane = ncvisual_blit(nc, visual, &opts);
        return true;
    } else {
        return false;
    }
}
