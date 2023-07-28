#include "label.h"
#include "util.h"

struct label {
    int y, x;
    unsigned int width, height;
    const char *text;
    struct ncplane *plane;
};

struct label *create_label(struct ncplane *parent, const int y, const int x, 
                           const unsigned int height, const unsigned int width, const char *text)
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
        struct label *label = malloc(sizeof(struct label));
        if (label == NULL)
            return NULL;
        label->y = y;
        label->x = x;
        label->height = height;
        label->width = width;
        label->plane = plane;
        label->text = text; 
        return label;
    }
}

struct label *create_label_without_ncplane(struct ncplane *parent, const int y, const int x, 
                                           const unsigned int height, const unsigned int width, const char *text)
{

}

void delete_label(struct label *label)
{
    ncplane_destroy(label->plane);
    label->plane = NULL;

    free(label);
    label = NULL;
}

void draw_label(struct label *label)
{
    ncplane_putstr(label->plane, label->text); 
}

void draw_label_box(struct label *label, unsigned int mask)
{
    create_box(label->plane, label->height- 1, label->width - 1, mask);
}

void draw_label_and_box(struct label *label, unsigned int mask)
{
    create_box(label->plane, label->height- 1, label->width - 1, mask);
    ncplane_putstr_yx(label->plane, label->height / 2, label->x, label->text); 
}
