#include "panel_list.h"
#include "shad_error.h"

struct panel_node {
    struct panel_node *prev;
    struct panel_node *next;
    struct ncplane *panel;

    panel_input_callback input_callback;
};

struct panel_list {
    struct panel_node *first;
    struct panel_node *last;
};

struct panel_list *create_list(void)
{
    struct panel_list *list = malloc(sizeof(struct panel_list));
    if (list == NULL) {
        return NULL;
    } else {
        list->first = NULL;
        list->last = NULL;
        return list;
    }
}

void destroy_list(struct panel_list *list)
{
    struct panel_node *tmp = NULL;
    while (list->first) {
        tmp = list->first;
        list->first = list->first->next;
        /* We don't destroy ncplane's, because notcurses will clean up itself */
        free(tmp);
        tmp = NULL;
    }
    free(list);
}

void clean_list_and_free(struct panel_list *list) 
{
    struct panel_node *tmp = NULL;
    while (list->first) {
        tmp = list->first;
        list->first = list->first->next;
        /* We don't destroy ncplane's, because notcurses will clean up itself */
        free(tmp);
        tmp = NULL;
    }
    list->first = NULL;
    list->last = NULL;
}

struct panel_node *panel_list_begin(struct panel_list *list)
{
    return list->first;
}

struct panel_node *panel_list_end(struct panel_list *list)
{
    return list->last;
}

struct panel_node *panel_list_next(struct panel_node *node)
{
    return node->next;
}


panel_input_callback get_panel_callback(struct panel_node *node)
{
    return node->input_callback;
}

struct ncplane *get_panel(struct panel_node *node)
{
    return node->panel;
}


void add_panel_to_list(struct panel_list *list, struct ncplane *panel, panel_input_callback callback)
{
    const char *error_msg = "Error on malloc in add_panel_to_list";
    struct panel_node *tmp = malloc(sizeof(struct panel_node));
    if (tmp == NULL)
        die_and_log(error_msg);
    tmp->panel = panel;
    tmp->input_callback = callback;
    tmp->prev = NULL;
    tmp->next = list->first;
    if (list->first)
        list->first->prev = NULL;
    else
        list->last = tmp;
    list->first = tmp;
}
