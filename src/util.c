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
