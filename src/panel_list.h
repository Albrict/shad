#pragma once
#include <notcurses/notcurses.h>

typedef void (*panel_input_callback)(const ncinput *input, struct ncplane *panel);

struct panel_node;
struct panel_list;

/* Allocates double-linked list using panel_node */
struct panel_list *create_list(void);
/* Delete list and all it's content */
void destroy_list(struct panel_list *list);
/* Delete all objects, but don't destroys a list */
void clean_list_and_free(struct panel_list *list);

void add_panel_to_list(struct panel_list *list, struct ncplane *panel, panel_input_callback input_callback);

struct panel_node *panel_list_begin(struct panel_list *list);
struct panel_node *panel_list_end(struct panel_list *list);
struct panel_node *panel_list_next(struct panel_node *node);

panel_input_callback get_panel_callback(struct panel_node *node);
struct ncplane *get_panel(struct panel_node *node);
