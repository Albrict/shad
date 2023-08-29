#include "ncpanel.h"

struct ncpanel_observer {
    void *data;
    bool is_notifyed;
    enum NCPANEL_OBSERVER_EVENT event;
};

struct ncpanel_observer_node {
    struct ncpanel_observer_node *prev;
    struct ncpanel_observer_node *next;
    struct ncpanel_observer *observer;
};

struct ncpanel_observer_list {
    struct ncpanel_observer_node *first;
    struct ncpanel_observer_node *last;
};

struct ncpanel_subject {
    struct ncpanel_observer_list *observers; 
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

struct ncpanel {
    int y, x;
    unsigned int rows, cols;

    struct ncplane *plane;
    struct ncpanel_list *childs;
    struct ncpanel_observer *observer;
    struct ncpanel_subject *subject;

    ncpanel_input_callback input_callback; 
    ncpanel_update_callback update_callback;
    ncpanel_clean_up_callback clean_up_callback;

    void *data;
    void *update_data;
    void *clean_up_data; 
};

static struct ncpanel_observer *ncpanel_create_observer(void);
static struct ncpanel_observer_list *ncpanel_create_observer_list();
static struct ncpanel_subject *ncpanel_create_subject(void);

static void ncpanel_destroy_observer(struct ncpanel_observer *observer);
static void ncpanel_clean_observer_list(struct ncpanel_observer_list *list);
static void ncpanel_destroy_subject(struct ncpanel_subject *subject);
static bool ncpanel_add_observer_to_list(struct ncpanel_observer_list *observer_list, struct ncpanel_observer *observer);

static struct ncpanel_list *ncpanel_create_list(void);
static void ncpanel_destroy_list(struct ncpanel_list *list);
static void ncpanel_clean_list_and_free(struct ncpanel_list *list);
static bool ncpanel_add_panel_to_list(struct ncpanel_list *list, struct ncpanel *panel);

static struct ncpanel *ncpanel_get_panel(struct ncpanel_node *node);
static struct ncpanel_node *ncpanel_list_begin(struct ncpanel_list *list);
static struct ncpanel_node *ncpanel_list_end(struct ncpanel_list *list);
static struct ncpanel_node *ncpanel_list_next(struct ncpanel_node *node);

struct ncpanel *ncpanel_create(struct ncplane *parent, const int y, const int x, const unsigned int rows, const int cols)
{
    struct ncpanel *panel = malloc(sizeof(struct ncpanel));
    if (panel) {
        struct ncplane_options opts;
        memset(&opts, 0, sizeof(struct ncplane_options));

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
            
            panel->childs = NULL;
            panel->observer = ncpanel_create_observer();
            if (!panel->observer)
                goto observer_err;
            panel->subject = ncpanel_create_subject();
            if (!panel->subject)
                goto subject_err;

            panel->input_callback = NULL;
            panel->update_callback = NULL;
            panel->clean_up_callback = NULL;

            panel->data = NULL;
            panel->update_data = NULL;
            panel->clean_up_data = NULL;
        } else {
            free(panel);
            panel = NULL;
            return panel;
        }
    }
    return panel;

    subject_err:
        ncpanel_destroy_observer(panel->observer);
    observer_err:
        ncplane_destroy(panel->plane);
        free(panel);
        return NULL;
}

struct ncpanel *ncpanel_wrap_plane(struct ncplane *plane, const int y, const int x, const unsigned int rows, const unsigned int cols)
{
    struct ncpanel *panel = malloc(sizeof(struct ncpanel));
    if (panel) {
        panel->y = y;
        panel->x = x;
        panel->rows = rows;
        panel->cols = cols;

        panel->plane = plane;
        panel->childs = NULL;
        panel->observer = ncpanel_create_observer();
        if (!panel->observer)
            goto observer_err;
        panel->subject = ncpanel_create_subject();
        if (!panel->subject)
            goto subject_err;

        panel->input_callback = NULL;
        panel->update_callback = NULL;
        panel->clean_up_callback = NULL;

        panel->data = NULL;
        panel->update_data = NULL;
        panel->clean_up_data = NULL;
        
    }
    return panel;

subject_err:
    ncpanel_destroy_observer(panel->observer);
observer_err:
    ncplane_destroy(panel->plane);
    free(panel);
    return NULL;
}

void ncpanel_destroy(struct ncpanel *panel)
{
    if (panel->clean_up_callback)
        panel->clean_up_callback(panel, panel->clean_up_data);
    if (panel->plane)
        ncplane_destroy(panel->plane);
    panel->plane = NULL;
    
    if (panel->childs)
        ncpanel_clean_list_and_free(panel->childs);

    ncpanel_destroy_observer(panel->observer);
    panel->observer = NULL;

    ncpanel_destroy_subject(panel->subject);
    panel->subject = NULL;

    free(panel);
    panel = NULL;
}

bool ncpanel_bind_observer_and_subject(struct ncpanel *observer, struct ncpanel *subject)
{
    return ncpanel_add_observer_to_list(subject->subject->observers, observer->observer);
}

void ncpanel_bind_update_callback(struct ncpanel *panel, ncpanel_update_callback callback, void *update_data)
{
    panel->update_callback = callback;
    panel->update_data = update_data;
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

void ncpanel_proccess_input_and_update(struct ncpanel *panel, const struct ncinput *input)
{
    if (panel->input_callback) {
        panel->input_callback(panel, input, panel->data); 
    } 
    if (panel->update_callback) {
        panel->update_callback(panel, panel->update_data);
    }
    if (panel->childs) {
        struct ncpanel_node *first = ncpanel_list_begin(panel->childs);
        while (first) {
            ncpanel_proccess_input_and_update(ncpanel_get_panel(first), input);
            first = ncpanel_list_next(first);
        }
    }
}

void ncpanel_update(struct ncpanel *panel)
{
    if (panel->update_callback)
        panel->update_callback(panel, panel->update_data);
}

void ncpanel_notify_observers(struct ncpanel *panel, void *data, enum NCPANEL_OBSERVER_EVENT event)
{
    if (panel->subject->observers) {
        struct ncpanel_observer_list *list = panel->subject->observers;
        struct ncpanel_observer_node *tmp = list->first;
        while(tmp) {
            tmp->observer->data = data;
            tmp->observer->is_notifyed = true;
            tmp->observer->event = event;
            tmp = tmp->next;
        }
    }
}

struct ncplane *ncpanel_get_plane(struct ncpanel *panel)
{
    return panel->plane;
}

void ncpanel_get_event_and_data(struct ncpanel *observer, void *data, enum NCPANEL_OBSERVER_EVENT *event)
{
    observer->observer->is_notifyed = false;
    data = observer->observer->data;
    *event = observer->observer->event;
}

void *ncpanel_get_data(struct ncpanel *observer)
{
    observer->observer->is_notifyed = false;
    return observer->observer->data;
}

enum NCPANEL_OBSERVER_EVENT ncpanel_get_event(struct ncpanel *observer)
{
    observer->observer->is_notifyed = false;
    return observer->observer->event;
}

unsigned int ncpanel_get_rows(const struct ncpanel *panel)
{
    return panel->rows;
}

unsigned int ncpanel_get_cols(const struct ncpanel *panel)
{
    return panel->cols;
}

bool ncpanel_is_notifyed(const struct ncpanel *observer)
{
    return observer->observer->is_notifyed;
}

bool ncpanel_add_child(struct ncpanel *parent, struct ncpanel *child)
{
    if (parent->childs) {
        return ncpanel_add_panel_to_list(parent->childs, child);
    } else {
        parent->childs = ncpanel_create_list();
        if (!parent->childs) {
            return false;
        } else {
            return ncpanel_add_panel_to_list(parent->childs, child);
        }
    }
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

bool ncpanel_blit_image_from_file(const char *filename, struct ncpanel *panel, const ncblitter_e blitter_type)
{
    struct ncvisual_options opts; 
    memset(&opts, 0, sizeof(struct ncvisual_options));

    opts.n = panel->plane;
    opts.x = 0;
    opts.y = 0;
    opts.begy = 0;
    opts.begx = 0;
    opts.lenx = ncplane_dim_x(panel->plane);
    opts.leny = ncplane_dim_y(panel->plane); 
    opts.blitter = blitter_type;
    opts.pxoffx = 0;
    opts.pxoffy = 0;

    struct ncvisual *visual = ncvisual_from_file(filename);
    if (visual) {
        ncvisual_resize(visual, opts.leny, opts.lenx);
        ncvisual_blit(ncplane_notcurses(panel->plane), visual, &opts);
        return true;
    } else {
        return false;
    }
}

static struct ncpanel_list *ncpanel_create_list(void)
{
    struct ncpanel_list *list = malloc(sizeof(struct ncpanel_list));
    if (list) {
        list->first = NULL;
        list->last = NULL;
    }
    return list;
}

static void ncpanel_destroy_list(struct ncpanel_list *list)
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

static void ncpanel_clean_list_and_free(struct ncpanel_list *list)
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

static bool ncpanel_add_panel_to_list(struct ncpanel_list *list, struct ncpanel *panel)
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

static struct ncpanel *ncpanel_get_panel(struct ncpanel_node *node)
{
    return node->panel;
}

static struct ncpanel_node *ncpanel_list_begin(struct ncpanel_list *list)
{
    return list->first;
}

static struct ncpanel_node *ncpanel_list_end(struct ncpanel_list *list)
{
    return list->last;
}

static struct ncpanel_node *ncpanel_list_next(struct ncpanel_node *node)
{
    return node->next;
}

static struct ncpanel_observer *ncpanel_create_observer(void)
{
    struct ncpanel_observer *observer = malloc(sizeof(struct ncpanel_observer));
    if (observer) {
        observer->data = NULL;
        observer->is_notifyed = false;
        observer->event = NCPANEL_EVENT_NONE;
    }
    return observer;
}

static struct ncpanel_observer_list *ncpanel_create_observer_list()
{
    struct ncpanel_observer_list *list = malloc(sizeof(struct ncpanel_observer_list));
    if (list) {
        list->last = NULL;
        list->first = NULL;
    }
    return list;
}

static struct ncpanel_subject *ncpanel_create_subject(void)
{
    struct ncpanel_subject *subject = malloc(sizeof(struct ncpanel_subject));
    if (subject) {
        subject->observers = ncpanel_create_observer_list(); 
        if (!subject->observers) {
            free(subject);
            subject = NULL;
            return NULL;
        }
    }
    return subject;
}

static void ncpanel_destroy_observer(struct ncpanel_observer *observer)
{
    observer->data = NULL;
    free(observer);
}

static void ncpanel_clean_observer_list(struct ncpanel_observer_list *list)
{
    list->first = NULL;
    list->last = NULL;
}

static void ncpanel_destroy_subject(struct ncpanel_subject *subject)
{
    ncpanel_clean_observer_list(subject->observers);
    subject->observers = NULL;

    free(subject);
    subject = NULL;
}

static bool ncpanel_add_observer_to_list(struct ncpanel_observer_list *observer_list, struct ncpanel_observer *observer)
{
    struct ncpanel_observer_node *tmp = malloc(sizeof(struct ncpanel_observer_node));
    if (tmp) {
        tmp->observer = observer;
        tmp->prev = NULL;
        tmp->next = observer_list->first;
        if (observer_list->first)
            observer_list->first->prev = NULL;
        else
            observer_list->last = tmp;
        observer_list->first = tmp;
        return true;
    } else {
        return false;
    }
}
