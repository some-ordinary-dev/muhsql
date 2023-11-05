#ifndef MUH_TEXT
#define MUH_TEXT

#include "box.h"

typedef struct muh_text {
    struct muh_box box;
    struct muh_panel *parent_panel;
    const char *text;
} muh_text;

void draw_text(muh_text *self);

muh_text *new_text(struct muh_point pt, const char *text, struct muh_panel *parent);

#endif
