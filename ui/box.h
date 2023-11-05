#ifndef MUH_BOX
#define MUH_BOX

struct muh_point {
    int x, y;
};

struct muh_box {
    struct muh_point p1;
    struct muh_point p2;
    void (*on_click)(struct muh_box *self);
};

int pt_in_box(struct muh_box b, struct muh_point pt);

void draw_border_box(struct muh_box *r);

#endif
