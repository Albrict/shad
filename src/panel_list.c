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
    if (last) {
        tmp->next = NULL;
        tmp->prev = last;
        last->next = tmp;

        tmp->panel = panel;
        tmp->input_callback = callback;
    } else {
        tmp->next = NULL;
        tmp->prev = first;

        tmp->panel = panel;
        tmp->input_callback = callback;

        first = last = tmp;
    }
}

struct panel_node *get_first_node(void)
{
    return first;
}
