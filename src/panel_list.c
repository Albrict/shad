#include "panel_list.h"
#include "shad_error.h"

static struct panel_node *first = NULL;
static struct panel_node *last = NULL;

void add_panel_to_list(struct ncplane *panel, panel_input_callback callback)
{
    const char *error_msg = "Error on malloc in add_panel_to_list";
    struct panel_node *tmp = malloc(sizeof(struct panel_node));
    if (tmp == NULL)
        die_and_log(error_msg);
    tmp->panel = panel;
    tmp->input_callback = callback;
    tmp->prev = NULL;
    tmp->next = first;
    if (first)
        first->prev = NULL;
    else
        last = tmp;
    first = tmp;
}

void free_panel_list(void)
{
    struct panel_node *tmp = NULL;
    while (first) {
        tmp = first;
        first = first->next;
        /* No need to destroy plane - on exit notcurses will clean up itself */
        free(tmp);
    }
}

struct panel_node *get_first_node(void)
{
    return first;
}
