#include "panel.h"
#include "box.h"
#include "command_center.h"
#include "unicode.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>

struct muh_panel *new_panel(struct muh_box box, char *title) {
    size_t len = strlen(title) + 1;

    muh_panel *p = malloc(sizeof(muh_panel));
    p->b = box;

    p->title = malloc(sizeof(char) * len);
    memset(p->title, '\0', len);
    strncpy(p->title, title, len - 1);
    
    return p;
}

void draw_panel(muh_panel *self) {
    draw_border_box((struct muh_box *)self);

    int y = self->b.p1.y;
    int x = self->b.p1.x + 3;

    mvaddwstr(y, x - 1, BOX_MIDDLE_SPLIT_LEFT);
    mvaddstr(y, x, self->title);
    mvaddwstr(y, x + strlen(self->title), BOX_MIDDLE_SPLIT_RIGHT);
}
