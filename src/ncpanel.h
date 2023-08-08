#pragma once
#include <notcurses/notcurses.h>

enum NCPANEL_OBSERVER_EVENT {
    NCPANEL_EVENT_NONE,
    NCPANEL_EVENT_CHANGED_COLOR,
    NCPANEL_EVENT_EXIT,
    NCPANEL_EVENT_SAVE
};

struct ncpanel;
struct ncpanel_node;
struct ncpanel_list;

typedef void (*ncpanel_input_callback)(struct ncpanel*, const struct ncinput*, void *data);
typedef void (*ncpanel_update_callback)(struct ncpanel*, void *update_data);
typedef void (*ncpanel_clean_up_callback)(struct ncpanel*, void *clean_up_data);

/* ncpanel operations */
struct ncpanel *ncpanel_create(struct ncplane *parent, const int y, const int x, const unsigned int rows, const int cols);
/* wraps ncplane into ncpanel */
struct ncpanel *ncpanel_wrap_plane(struct ncplane *plane, const int y, const int x, const unsigned int rows, const unsigned int cols);
void ncpanel_destroy(struct ncpanel *panel);

void ncpanel_bind_observer_and_subject(struct ncpanel *observer, struct ncpanel *subject);
void ncpanel_bind_update_callback(struct ncpanel *panel, ncpanel_update_callback callback, void *update_data);
void ncpanel_bind_input_callback(struct ncpanel *panel, ncpanel_input_callback callback, void *data);
void ncpanel_bind_clean_up_callback(struct ncpanel *panel, ncpanel_clean_up_callback callback, void *clean_up_data);

void ncpanel_update(struct ncpanel *panel);
void ncpanel_proccess_input(struct ncpanel *panel, const struct ncinput *input);
void ncpanel_proccess_input_and_update(struct ncpanel *panel, const struct ncinput *input);
void ncpanel_notify_observers(struct ncpanel *panel, void *data, enum NCPANEL_OBSERVER_EVENT event);

struct ncplane *ncpanel_get_plane(struct ncpanel *observer);
void ncpanel_get_event_and_data(struct ncpanel *observer, void *data, enum NCPANEL_OBSERVER_EVENT *event);
void *ncpanel_get_data(struct ncpanel *observer);
enum NCPANEL_OBSERVER_EVENT ncpanel_get_event(struct ncpanel *observer);
bool ncpanel_is_notifyed(const struct ncpanel *observer);

void ncpanel_set_input_data(struct ncpanel *panel, void* new_data);
/* ncpanel list operations */
struct ncpanel_list *ncpanel_create_list(void);
/* Delete list and all it's content */
void ncpanel_destroy_list(struct ncpanel_list *list);
/* Delete all objects, but don't destroys a list */
void ncpanel_clean_list_and_free(struct ncpanel_list *list);

bool ncpanel_add_panel_to_list(struct ncpanel_list *list, struct ncpanel *panel);

struct ncpanel *ncpanel_get_panel(struct ncpanel_node *node);
struct ncpanel_node *ncpanel_list_begin(struct ncpanel_list *list);
struct ncpanel_node *ncpanel_list_end(struct ncpanel_list *list);
struct ncpanel_node *ncpanel_list_next(struct ncpanel_node *node);

/* Graphics */
int ncpanel_create_box(struct ncpanel *panel, const int rows, const int cols, unsigned int mask);
