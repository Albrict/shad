#pragma once
#include <notcurses/notcurses.h>

typedef void (*panel_input_callback)(const ncinput *input, struct ncplane *panel);

struct panel_node {
    struct panel_node *prev;
    struct panel_node *next;
    struct ncplane *panel;

    panel_input_callback input_callback;
};

void add_panel_to_list(struct ncplane *panel, panel_input_callback callback);
void free_panel_list(void);

struct panel_node *get_first_node(void);
