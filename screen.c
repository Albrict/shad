#include "shad.h"
#include <curses.h>

/* Messages */
extern const char err_init_screen[];
extern const char err_init_colors[]; 
extern const char err_init_mouse[]; 

const char msg_current_color[] = "Current color:";

/* Constants and macros */
#define BAR_HEIGHT 6

const char report_mouse[] = "\033[?1003h\n";
/* Where we want to draw */
static WINDOW *main_field;
/* Here will be displayed colors and tools */
static WINDOW *tool_bar;

/* Getters */
WINDOW *get_field()
{
    return main_field;
}

WINDOW *get_bar()
{
    return tool_bar;
}

/* Window functions */
void update()
{
    wrefresh(get_bar());
    wrefresh(get_field());
}

void end_screen()
{
    delwin(get_field());
    delwin(get_bar());
    endwin();
}

void set_current_color()
{

}

/* Initizalition */
static void init_field()
{
    /* Height, width, starty, startx */
    main_field = newwin(LINES - 6, COLS, 0, 0);
    keypad(get_field(), true);
}

static void init_bar()
{
    int i = COLOR_RED;
    int y = 1, x = getmaxx(get_field()) - 10;
    int msg_x = x - sizeof(msg_current_color) - 1;
    /* Height, width, starty, startx */
    tool_bar = newwin(BAR_HEIGHT, COLS, LINES - 6, 0); 
    box(get_bar(), 0, 0);

    /* Now time to draw our color pallete */
    for (; i < COLOR_WHITE + 1; i++) {
        if (y == getmaxy(get_bar()) - 1) {
            x++;
            y = 1;
        }
        change_color(i, get_bar());
        mvwaddch(get_bar(), y, x, ' ');
        y++;
    }
    mvwaddstr(get_bar(), 1, msg_x, msg_current_color);
}

static void init_mouse()
{
    /* Makes terminal report mouse movements */
    printf(report_mouse);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);
}

void init_shad()
{
    if (!initscr()) 
        die(err_init_screen);
    if (!has_colors()) {
        die(err_init_colors);
    }
    else {
        start_color();
        init_shad_colors();
    }
    noecho(); 
    curs_set(0);
    /* We need this to have non blocking input */
    halfdelay(1);
    refresh();

    init_field();
    init_bar();
    update();
   
    init_mouse();
}
