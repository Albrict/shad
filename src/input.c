#include "input.h"

static void proccess_mouse(struct panel_list *list, const ncinput *input);

void proccess_input(struct panel_list *list, const ncinput *input)
{
    if (nckey_mouse_p(input->id) == true)
        proccess_mouse(list, input);
}

static void proccess_mouse(struct panel_list *list, const ncinput *input)
{
    struct panel_node *first = panel_list_begin(list);
    while (first) {
        panel_input_callback callback = get_panel_callback(first);
        if (callback)
            callback(input, get_panel(first));
        first = panel_list_next(first);
    }
}
