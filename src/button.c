#include "button.h"
#include "util.h"

struct button {
    int y, x;
    unsigned int width, height;
    struct ncplane *plane;
};

struct button *create_button(struct ncplane *parent, const int y, const int x, const unsigned int height, const unsigned int width)
{
    struct ncplane_options opts;
    struct ncplane *plane = NULL;
    memset(&opts, 0, sizeof(opts));
    
    opts.y = y;
    opts.x = x;
    opts.cols = width;
    opts.rows = height;
    
    plane = ncplane_create(parent, &opts);
    if (plane == NULL) {
        return NULL;
    } else {
        struct button *button = malloc(sizeof(struct button));
        button->y = y;
        button->x = x;
        button->height = height;
        button->width = width;
        button->plane = plane;
        return button;
    }
}

void delete_button(struct button *button)
{
    ncplane_destroy(button->plane);
    button->plane = NULL;

    free(button);
    button = NULL;
}

void draw_box(struct button *button, unsigned int mask)
{
    create_box(button->plane, button->height- 1, button->width - 1, mask);
}
