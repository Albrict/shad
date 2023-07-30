#include "input.h"
#include "panel_list.h"

static void proccess_mouse(const ncinput *input);

void proccess_input(const ncinput *input)
{
    if (nckey_mouse_p(input->id) == true)
        proccess_mouse(input);
}

static void proccess_mouse(const ncinput *input)
{
    struct panel_node *first = get_first_node();
    while (first) {
        first->input_callback(input, first->panel);
        first = first->next;
    }
}
