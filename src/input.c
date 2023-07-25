#include "input.h"
#include "canvas.h"
#include "instrument_panel.h"

static void proccess_mouse(const ncinput *input);

void proccess_input(const ncinput *input)
{
    if (nckey_mouse_p(input->id) == true)
        proccess_mouse(input);
}

static void proccess_mouse(const ncinput *input)
{
    int y = input->y;
    int x = input->x;
    if (ncplane_translate_abs(get_canvas_plane(), &y, &x) == true)
        proccess_input_on_canvas(input);

    if (ncplane_translate_abs(get_instument_panel_plane(), &y, &x) == true)
        proccess_input_on_instrument_panel_plane(input);
}
