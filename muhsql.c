#include <curses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_null.h>
#include <unctrl.h>

#include "db.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/command_center.h"
#include "ui/panel.h"
#include "ui/query_window.h"
#include "ui/tree_viewer.h"
#include "ui/window.h"
#include "util.h"

#define MAX_PANELS 50

muh_panel *panels[MAX_PANELS];
int num_panels = 0;

void draw_panels(void) {
    for (int i = 0; i < num_panels; ++i) {
        draw_panel(panels[i]);
    }
}

void cleanup(void) {
    // if (db != NULL) {
    //     close_db(db);
    // }
    endwin();
}

muh_panel *get_current_panel(void) {
    int y, x;
    getyx(stdscr, y, x);

    struct muh_point pt = {x, y};

    muh_panel *p;
    for (int i = 0; i < num_panels; ++i) {
        p = panels[i];
        if (pt_in_box(p->b, pt)) {
            return p;
        }
    }

    return NULL;
}

muh_panel *get_right_neighbour(muh_panel *cur_panel) {
    muh_panel *p, *right_panel = NULL;

    for (int i = 0; i < num_panels; ++i) {
        p = panels[i];
        // panel is to the right of the current panel
        if (p != cur_panel && p->b.p1.x > cur_panel->b.p2.x) {
            // if we don't have a baseline or this panel is closer than the
            // baseline
            if (right_panel == NULL || right_panel->b.p1.x > p->b.p1.x) {
                right_panel = p;
            }
        }
    }

    return right_panel;
}

muh_panel *get_up_neighbour(muh_panel *cur_panel) {
    muh_panel *p, *up_panel = NULL;

    for (int i = 0; i < num_panels; ++i) {
        p = panels[i];
        // panel is above the current panel
        if (p != cur_panel && p->b.p2.y < cur_panel->b.p1.y) {
            // if we don't have a baseline or this panel is closer than the
            // baseline
            if (up_panel == NULL || p->b.p2.y > up_panel->b.p2.y) {
                up_panel = p;
            }
        }
    }

    return up_panel;
}

muh_panel *get_down_neighbour(muh_panel *cur_panel) {
    muh_panel *p, *down_panel = NULL;

    for (int i = 0; i < num_panels; ++i) {
        p = panels[i];
        // panel is below the current panel
        if (p != cur_panel && p->b.p1.y > cur_panel->b.p2.y) {
            // if we don't have a baseline or this panel is closer than the
            // baseline
            if (down_panel == NULL || p->b.p1.y < down_panel->b.p1.y) {
                down_panel = p;
            }
        }
    }

    return down_panel;
}

muh_panel *get_left_neighbour(muh_panel *cur_panel) {
    muh_panel *p, *left_panel = NULL;

    for (int i = 0; i < num_panels; ++i) {
        p = panels[i];
        // panel is to the left of the current panel
        if (p != cur_panel && p->b.p2.x < cur_panel->b.p1.x) {
            // if we don't have a baseline or this panel is closer than the
            // baseline
            if (left_panel == NULL || left_panel->b.p2.x < p->b.p2.x) {
                left_panel = p;
            }
        }
    }

    return left_panel;
}

typedef enum {
    FLAG_NONE = 0,
    FLAG_START_PANEL_MOVE = 1,
} flags;

int main(void) {
    atexit(cleanup);

    // init curses
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    keypad(stdscr, true);
    noecho();

    // setup global vars
    getmaxyx(stdscr, MAX_Y, MAX_X);

    MAX_Y -= 1;
    MAX_X -= 1;

    // init screen layout
    init_cmd_center();
    muh_panel *tree_viewer = init_tree_viewer();
    muh_panel *query_window = init_query_window();

    // panels[num_panels++] = cmd_center;
    panels[num_panels++] = tree_viewer;
    panels[num_panels++] = query_window;

    draw_panels();

    // get handle to DB
    muh_db *db;
    open_db(&db);

    // print tables
    char **tables;
    int rc = get_tables(db, &tables);

    for (int i = 0; i < rc; ++i) {
        mvaddstr(i + 1, 1, tables[i]);
    }

    int cur_x, cur_y, reset_flag;
    char ch, last_ch;
    char *unctrl_ch;
    flags f = FLAG_NONE;
    muh_panel *current_panel;
    while ((ch = getch()) != EOF) {
        reset_flag = 1;
        current_panel = get_current_panel();
        getyx(stdscr, cur_y, cur_x);

        unctrl_ch = unctrl(ch);
        log_msg(unctrl_ch);

        if (strlen(unctrl_ch) > 1) {
            // handle control characters
            switch (unctrl_ch[1]) {
            case 'D': { // move down half a panel
                int height = current_panel->b.p2.y - current_panel->b.p1.y;
                int step = height / 2;
                cur_y = cur_y + step;
                cur_y = MIN(cur_y, current_panel->b.p2.y - 1);
            } break;
            case 'B': { // move up half a panel
                int height = current_panel->b.p2.y - current_panel->b.p1.y;
                int step = height / 2;
                cur_y = cur_y - step;
                cur_y = MAX(cur_y, current_panel->b.p1.y + 1);
            } break;
            case 'W': { // start move to another panel
                f = FLAG_START_PANEL_MOVE;
                reset_flag = 0;
            } break;
            case 'J': { // enter key press.
                // TODO: this also will trigger if CTRL-J is pressed, which
                // isn't really what we want. Not sure how we could distinguish
                // between the two...

                // get current element and trigger click event
            } break;
            }
        } else {
            // handle plain characters
            switch (ch) {

                /* start vim bindings */
            case 'j':
            case 2: // down arrow
                // move down
                if (f == FLAG_START_PANEL_MOVE) {
                    // find next panel above this panel
                    muh_panel *neighbour = get_down_neighbour(current_panel);
                    if (neighbour != NULL) {
                        cur_y = neighbour->b.p1.y + 1;
                        current_panel = neighbour;
                    }
                } else {
                    cur_y = cur_y + 1;
                }
                break;
            case 'k':
            case 3: // up arrow
                // move up
                if (f == FLAG_START_PANEL_MOVE) {
                    // find next panel above this panel
                    muh_panel *neighbour = get_up_neighbour(current_panel);
                    if (neighbour != NULL) {
                        cur_y = neighbour->b.p2.y - 1;
                        current_panel = neighbour;
                    }
                } else {
                    cur_y = cur_y - 1;
                }
                break;
            case 'l':
            case 5: { // right arrow
                // move right
                if (f == FLAG_START_PANEL_MOVE) {
                    // find next panel to the right
                    muh_panel *neighbour = get_right_neighbour(current_panel);
                    if (neighbour != NULL) {
                        cur_x = neighbour->b.p1.x + 1;
                        current_panel = neighbour;
                    }
                } else {
                    cur_x = cur_x + 1;
                }
            } break;
            case 'h':
            case 4: // left arrow
                // move left
                if (f == FLAG_START_PANEL_MOVE) {
                    // find next panel to the left
                    muh_panel *neighbour = get_left_neighbour(current_panel);
                    if (neighbour != NULL) {
                        cur_x = neighbour->b.p2.x - 1;
                        current_panel = neighbour;
                    }
                } else {
                    cur_x = cur_x - 1;
                }
                break;
            case 'g':
                // move to top
                if (last_ch == 'g') {
                    cur_y = current_panel->b.p1.y + 1;
                }
                break;
            case 'G':
                // move to bottom
                cur_y = current_panel->b.p2.y - 1;
                break;
            case '0':
                // move to start of line
                cur_x = current_panel->b.p1.x + 1;
                break;
            case '$':
                // move to end of line
                cur_x = current_panel->b.p2.x - 1;
                break;
                /* end vim bindings */

            default:
                break;
            }
        }

        // only move if the new position is within the box
        struct muh_point new_pos = {cur_x, cur_y};
        if (pt_in_box(current_panel->b, new_pos)) {
            move(cur_y, cur_x);
        }

        if (reset_flag) {
            f = FLAG_NONE;
        }

        last_ch = ch;
    }

    return 0;
}
