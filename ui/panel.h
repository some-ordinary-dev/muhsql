#ifndef MUH_PANEL
#define MUH_PANEL

#include "box.h"

typedef struct muh_panel {
    struct muh_box b;
    char *title;
} muh_panel;

muh_panel *new_panel(struct muh_box box, char *title);

void draw_panel(struct muh_panel *self);

#endif
