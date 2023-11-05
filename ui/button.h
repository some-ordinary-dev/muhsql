#ifndef UI_BUTTON
#define UI_BUTTON

#include "box.h"

typedef struct muh_button {
    struct muh_box box;
    const char *label;
} muh_button;

struct muh_button *muh_button_create(struct muh_point p, const char *label);
void muh_button_free(struct muh_button *self);
void draw_button(struct muh_button *self);

#endif
