#include "text.h"
#include <stdlib.h>
#include <string.h>

void draw_text(muh_text *self) {}

muh_text *new_text(struct muh_point pt, const char *text,
                   struct muh_panel *parent) {
    // TODO: store every box with no parent in an array
    // TODO: add children to box
    // TODO: make sure every box that's a child is registered under the correct
    // parent
    // TODO: add helper function to register click event to box
    // TODO: when detectig click events we can start by finding the parent that
    // is under the click, then going through the children recursively
    muh_text *t = malloc(sizeof(muh_text));

    t->parent_panel = parent;
    t->box.p1.x = pt.x;
    t->box.p1.y = pt.y;
    t->box.p2.x = pt.x + strlen(text);
    t->box.p2.y = pt.y;

    return t;
}
