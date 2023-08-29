#include "button.h"

struct button {
    int y, x;
    unsigned int width, height;
    struct ncplane *plane;
    button_callback callback;
    void *data;
};

struct button *create_button(struct ncplane *parent, const int y, const int x, const unsigned int height, const unsigned int width)
{
    struct ncplane_options opts;
    struct ncplane *plane = NULL;
    memset(&opts, 0, sizeof(opts));
    
    opts.y = y;
    opts.x = x;
    opts.cols = width;
    opts.rows = height;
    
    plane = ncplane_create(parent, &opts);
    if (plane == NULL) {
        return NULL;
    } else {
        struct button *button = malloc(sizeof(struct button));
        if (button == NULL)
            return NULL;
        button->y = y;
        button->x = x;
        button->height = height;
        button->width = width;
        button->plane = plane;
        button->callback = NULL;
        button->data = NULL;
        return button;
    }
}

void delete_button(struct button *button)
{
    ncplane_destroy(button->plane);
    button->plane = NULL;

    free(button);
    button = NULL;
}

void bind_callback(struct button *button, button_callback callback, void *data)
{
    button->callback = callback;
    button->data = data;
}

void proccess_input_on_button(struct button *button, const ncinput *input)
{
    int mouse_y = input->y;
    int mouse_x = input->x;
    
    if (input->id == NCKEY_BUTTON1) {
        if (ncplane_translate_abs(button->plane, &mouse_y, &mouse_x) == true) {
            if (button->callback)
                button->callback(button->data);
        }
    }
}

void draw_button_box(struct button *button, unsigned int mask)
{
//    create_box(button->plane, button->height- 1, button->width - 1, mask);
}

void draw_text_on_button(struct button *button, const wchar_t *text)
{
    ncplane_putwstr_yx(button->plane, button->height / 2, button->width / 2 - wcslen(text) / 2, text); 
}
