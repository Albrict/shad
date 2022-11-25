#include "shad.h"
#include <curses.h>

/* Messages */
extern const char err_init_screen[];
extern const char err_init_colors[]; 
extern const char err_init_mouse[]; 

const char msg_current_color[] = "Current color:";
const char msg_current_tool[] = "Current tool:";
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
    static int x;
    x = getmaxx(stdscr) - 15;
    change_color(get_current_color(), get_bar());
    mvwaddch(get_bar(), 1, x, ' ');
}

static void draw_pallete()
{
    colors i = RED;
    int y = 3, x = getmaxx(get_field()) - 20;
    int msg_x = getmaxx(get_field()) - sizeof(msg_current_color) - 15;
    for (; i < COLOR_WHITE + 1; i++) {
        if (x == getmaxx(get_bar()) - 1) {
            y++;
            x = 1;
        }
        change_color(i, get_bar());
        mvwaddch(get_bar(), y, x, ' ');
        x++;
    }
    change_color(BLACK, get_bar());
    mvwaddstr(get_bar(), 1, msg_x, msg_current_color);
}

/*static void print_tools()
{
    int y = 3, x = 10; 
    
} */

/* Initizalition */
static void init_field()
{
    /* Height, width, starty, startx */
    main_field = newwin(LINES - 6, COLS, 0, 0);
    keypad(get_field(), true);
}


static void init_bar()
{
    /* Height, width, starty, startx */
    tool_bar = newwin(BAR_HEIGHT, COLS, LINES - 6, 0); 
    box(get_bar(), 0, 0);

    /* Now time to draw our color pallete */
    draw_pallete();
    mvwaddstr(get_bar(), 1, 15, msg_current_tool);
}

static void init_mouse()
{
    /* Makes terminal report mouse movements */
    printf(report_mouse);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);
}

static void init_terminal()
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

}

void load_field_and_init(FILE *field)
{
    init_terminal();
    main_field = getwin(field); 
    init_bar();
    init_mouse();

    update();
}

void init_shad()
{
    init_terminal();
    init_field();
    init_bar();
    update();
    
    init_mouse();
}
