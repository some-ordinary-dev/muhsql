#include "button.h"
#include "box.h"
#include "command_center.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>

static size_t click_count = 0;

static void default_on_click(struct muh_box *self) {
    char msg[50];
    sprintf(msg, "clicked button %lu!", click_count);
    click_count++;
    log_msg(msg);
}

void draw_button(struct muh_button *self) {
    draw_border_box((struct muh_box *)self);
    mvaddstr(self->box.p1.y + 1, self->box.p1.x + 1, self->label);
}

muh_button *muh_button_create(struct muh_point p, const char *label) {
    muh_button *btn = malloc(sizeof(muh_button));
    btn->label = label;
    btn->box.on_click = default_on_click;
    btn->box.p1 = p;
    btn->box.p2.x = p.x + strlen(label) + 1;
    btn->box.p2.y = p.y + 2;
    return btn;
}

void muh_button_free(struct muh_button *btn) { free(btn); }
