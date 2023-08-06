#include "ncpanel.h"


struct ncpanel {
    int y, x;
    unsigned int rows, cols;
    struct ncplane *plane;
    ncpanel_input_callback input_callback; 
    ncpanel_clean_up_callback clean_up_callback;
    void *data;
    void *clean_up_data; 
};

struct ncpanel_node {
    struct ncpanel_node *prev;
    struct ncpanel_node *next;
    struct ncpanel *panel;
};

struct ncpanel_list {
    struct ncpanel_node *first;
    struct ncpanel_node *last;
};

struct ncpanel *ncpanel_create(struct ncplane *parent, const int y, const int x, const unsigned int rows, const int cols)
{
    struct ncpanel *panel = malloc(sizeof(struct ncpanel));
    if (panel) {
        struct ncplane_options opts;
        memset(&opts, 0, sizeof(struct ncpanel));

        opts.y = y;
        opts.x = x;
        opts.rows = rows;
        opts.cols = cols;
        
        panel->plane = ncplane_create(parent, &opts);
        if (panel->plane) {
            panel->y = y;
            panel->x = x;
            panel->rows = rows;
            panel->cols = cols;
            panel->input_callback = NULL;
            panel->clean_up_callback = NULL;
            panel->clean_up_data = NULL;
        } else {
            free(panel);
            panel = NULL;
            return panel;
        }
    }
    return panel;
}

void ncpanel_destroy(struct ncpanel *panel)
{
    if (panel->clean_up_callback)
        panel->clean_up_callback(panel, panel->clean_up_data);
    ncplane_destroy(panel->plane);
    panel->plane = NULL;

    free(panel);
    panel = NULL;
}

void ncpanel_bind_input_callback(struct ncpanel *panel, ncpanel_input_callback callback, void *data)
{
    panel->input_callback = callback;
    panel->data = data;
}

void ncpanel_bind_clean_up_callback(struct ncpanel *panel, ncpanel_clean_up_callback callback, void *clean_up_data)
{
    panel->clean_up_callback = callback;
    panel->clean_up_data = clean_up_data;
}

int ncpanel_create_box(struct ncpanel *panel, const int rows, const int cols, unsigned int mask)
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

    nccell_load(panel->plane, &ul, "╭");
    nccell_load(panel->plane, &ur, "╮");
    nccell_load(panel->plane, &ll, "╰");
    nccell_load(panel->plane, &lr, "╯");
    nccell_load(panel->plane, &hline, "─");
    nccell_load(panel->plane, &vline, "│");

    return ncplane_box(panel->plane, &ul, &ur, &ll, &lr, &hline, &vline, rows, cols, mask); 
}


void ncpanel_proccess_input(struct ncpanel *panel, const struct ncinput *input)
{
    panel->input_callback(panel, input, panel->data); 
}

struct ncplane *ncpanel_get_plane(struct ncpanel *panel)
{
    return panel->plane;
}

struct ncpanel_list *ncpanel_create_list(void)
{
    struct ncpanel_list *list = malloc(sizeof(struct ncpanel_list));
    if (list) {
        list->first = NULL;
        list->last = NULL;
    }
    return list;
}

void ncpanel_destroy_list(struct ncpanel_list *list)
{
    struct ncpanel_node *tmp = NULL;
    while (list->first) {
        tmp = list->first;
        list->first = list->first->next;
        ncpanel_destroy(tmp->panel);
        tmp = NULL;
    }
    free(list);
}

void ncpanel_clean_list_and_free(struct ncpanel_list *list)
{
    struct ncpanel_node *tmp = NULL;
    while (list->first) {
        tmp = list->first;
        list->first = list->first->next;
        ncpanel_destroy(tmp->panel);
        tmp = NULL;
    }
    list->first = NULL;
    list->last = NULL;
}

bool ncpanel_add_panel_to_list(struct ncpanel_list *list, struct ncpanel *panel)
{
    struct ncpanel_node *tmp = malloc(sizeof(struct ncpanel_node));
    if (tmp) {
        tmp->panel = panel;
        tmp->prev = NULL;
        tmp->next = list->first;
        if (list->first)
            list->first->prev = NULL;
        else
            list->last = tmp;
        list->first = tmp;
        return true;
    } else {
        return false;
    }
}

struct ncpanel *ncpanel_get_panel(struct ncpanel_node *node)
{
    return node->panel;
}

struct ncpanel_node *ncpanel_list_begin(struct ncpanel_list *list)
{
    return list->first;
}

struct ncpanel_node *ncpanel_list_end(struct ncpanel_list *list)
{
    return list->last;
}

struct ncpanel_node *ncpanel_list_next(struct ncpanel_node *node)
{
    return node->next;
}
