#include "command_center.h"
#include "../util.h"
#include "panel.h"
#include "window.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define MSG_HISTORY 50
#define MAX_MSG_LEN (MAX_X - 2)
#define FIRST_AVAILABLE_MESSAGE (1)

// ring buffer of messages to be displayed
static char *messages[MSG_HISTORY];
static size_t num_messages;
static WINDOW *win;

muh_panel *init_cmd_center(void) {
    win = newwin(0, 0, COMMAND_CENTER_TOP, 0);
    box(win, 0, 0);
    wrefresh(win);
    return NULL;

    //
    // struct muh_box command_center_box = {
    //     {0, COMMAND_CENTER_TOP},
    //     {MAX_X, MAX_Y},
    // };
    //
    // return new_panel(command_center_box, "Command Log");
}

int normalize(int actual, int max) {
    if (actual < 0) {
        return actual + max;
    }

    return actual < max ? actual : normalize(actual - max, max);
}

void log_msg(const char *msg) {
    // create copy of the message so that we are sure to have valid memory
    size_t len = MIN(strlen(msg) + 1, MAX_MSG_LEN);
    char *copy = malloc(sizeof(char) * len);
    memset(copy, '\0', len);
    strncpy(copy, msg, len - 1);

    int normalized = normalize(num_messages, MSG_HISTORY);

    if (num_messages >= MSG_HISTORY) {
        // free previously logged message if we're writing over another message
        free(messages[normalized]);
    }

    messages[normalized] = copy;

    int last_y, last_x;
    getyx(stdscr, last_y, last_x);

    // clear command center
    for (int i = FIRST_AVAILABLE_MESSAGE; i < MAX_Y; ++i) {
        for (int j = 1; j < MAX_X; ++j) {
            mvwaddch(win, i, j, ' ');

        }
    }

    num_messages++;

    int base = MIN(num_messages, MAX_MESSAGES);
    for (int i = 0; i < base; ++i) {
        int normalized = normalize(num_messages - 1 - i, MSG_HISTORY);
        mvwaddstr(win, FIRST_AVAILABLE_MESSAGE + (base - 1 - i), 1,
                 messages[normalized]);
    }

    move(last_y, last_x);
    wrefresh(win);
}
