#include "shad.h"
#include <curses.h>
/* Enums, structs and etc */
typedef enum {
    OTHER,
    DRAW,
    ERASE,
} mouse_events;

/* Mouse functions */
static mouse_events procces_mouse_event(MEVENT *event)
{
    if (event->bstate & BUTTON1_PRESSED)
        return DRAW;
    if (event->bstate & BUTTON1_RELEASED)
        return DRAW;
    if (event->bstate & BUTTON3_PRESSED)
        return ERASE;
    else
        return OTHER;
}

static void procces_mouse()
{
    mouse_events key;
    MEVENT event;
    if (getmouse(&event) == OK) {
        key = procces_mouse_event(&event);
        switch(key) {
        case DRAW:
            if (getmouse(&event) == OK) {
                mvwaddch(get_field(), event.y, event.x, ACS_BLOCK);
            }
            break;
        case ERASE:
            mvwaddch(get_field(), event.y, event.x, '   ');
            break;
        case OTHER:
            ; 
        }
    }
}

/* Keyboard functions */
void procces_input()
{
    int key = wgetch(get_field());
    switch (key) {
    case ERR:
        return;
        break;
    case KEY_MOUSE:
        procces_mouse();
        break;
    case '0'...'9':
        ;
    case KEY_ESC:
        end_screen();
        exit(EXIT_SUCCESS);
        break;
    }
}
