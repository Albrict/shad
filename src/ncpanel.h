#pragma once
#include <notcurses/notcurses.h>

enum NCPANEL_OBSERVER_EVENT {
    NCPANEL_EVENT_NONE,
    NCPANEL_EVENT_CHANGED_COLOR,
    NCPANEL_EVENT_EXIT,
    NCPANEL_EVENT_SAVE
};

struct ncpanel;

typedef void (*ncpanel_input_callback)(struct ncpanel*, const struct ncinput*, void *data);
typedef void (*ncpanel_update_callback)(struct ncpanel*, void *update_data);
typedef void (*ncpanel_clean_up_callback)(struct ncpanel*, void *clean_up_data);



/* Create/destroy functions*/
struct ncpanel *ncpanel_create(struct ncplane *parent, const int y, const int x, const unsigned int rows, const int cols);             /* create ncpanel instance */
struct ncpanel *ncpanel_wrap_plane(struct ncplane *plane, const int y, const int x, const unsigned int rows, const unsigned int cols); /* wrap ncplane to ncpanel */
void ncpanel_destroy(struct ncpanel *panel); /* destroy all childs of ncpanel and itself and free memory */

/* Bind callback function to ncpanel*/
bool ncpanel_bind_observer_and_subject(struct ncpanel *observer, struct ncpanel *subject);                                             /* bind two ncpanels to have observer/subject relation */
void ncpanel_bind_update_callback(struct ncpanel *panel, ncpanel_update_callback callback, void *update_data);                         /* bind update callback for ncpanel */
void ncpanel_bind_input_callback(struct ncpanel *panel, ncpanel_input_callback callback, void *data);                                  /* bind input callback for ncpanel */ 
void ncpanel_bind_clean_up_callback(struct ncpanel *panel, ncpanel_clean_up_callback callback, void *clean_up_data);                   /* bind clean up callback(destructor) */

/* State functions*/
void ncpanel_update(struct ncpanel *panel);                                                                                            /* update ncpanel(use update callback, if bind one) */
void ncpanel_proccess_input_and_update(struct ncpanel *panel, const struct ncinput *input);                                            /* proccess input and then update */
void ncpanel_notify_observers(struct ncpanel *panel, void *data, enum NCPANEL_OBSERVER_EVENT event);                                   /* notify observers if have relation */

/* Getters */
struct ncplane *ncpanel_get_plane(struct ncpanel *panel);                                                                              /* returns ncplane */
void ncpanel_get_event_and_data(struct ncpanel *observer, void *data, enum NCPANEL_OBSERVER_EVENT *event);                             /* returns data and type of event that was send by subject */
void *ncpanel_get_data(struct ncpanel *observer);                                                                                      /* returns just data(that was send by subject) */
enum NCPANEL_OBSERVER_EVENT ncpanel_get_event(struct ncpanel *observer);                                                               /* returns only event that was send by subject */
bool ncpanel_is_notifyed(const struct ncpanel *observer);                                                                              /* checks if ncpanel is notifyed by subject */
unsigned int ncpanel_get_rows(const struct ncpanel *panel);                                                                            /* returns rows of ncpanel */
unsigned int ncpanel_get_cols(const struct ncpanel *panel);                                                                            /* returns cols of ncpanel */


/* Parent/child functions */
bool ncpanel_add_child(struct ncpanel *parent, struct ncpanel *child);                                                                  /* Add child to ncpanel. Returns false on failure */

/* Graphics */
int ncpanel_create_box(struct ncpanel *panel, const int rows, const int cols, unsigned int mask);                                      /* draw box around ncpanel */
bool ncpanel_blit_image_from_file(const char *filename, struct ncpanel *panel, const ncblitter_e blitter_type);                        /* blit image from file(using ncblit). Returns false on failure */
