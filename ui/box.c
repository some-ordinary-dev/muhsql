#include "box.h"
#include <curses.h>
#include <wchar.h>

int pt_in_box(struct muh_box b, struct muh_point pt) {
    return b.p1.x < pt.x && pt.x < b.p2.x && b.p1.y < pt.y && pt.y < b.p2.y;
}

void draw_border_box(struct muh_box *box) {
    // extract vars
    int topCornerX = box->p1.x;
    int topCornerY = box->p1.y;

    int bottomCornerX = box->p2.x;
    int bottomCornerY = box->p2.y;

    int xDist = box->p2.x - box->p1.x;
    int yDist = box->p2.y - box->p1.y;

    wchar_t xMiddle[xDist * 4];
    wcscpy(xMiddle, L"");
    for (int i = 0; i < xDist; ++i)
        wcscat(xMiddle, L"\u2500");

    wchar_t *yMiddle = {L"\u2502"};

    // draw top  border
    mvaddwstr(topCornerY, topCornerX, L"\u256D");
    mvaddwstr(topCornerY, topCornerX + 1, xMiddle);
    mvaddwstr(topCornerY, topCornerX + xDist, L"\u256E");

    // draw bottom border
    mvaddwstr(bottomCornerY, topCornerX, L"\u2570");
    mvaddwstr(bottomCornerY, topCornerX + 1, xMiddle);
    mvaddwstr(bottomCornerY, topCornerX + xDist, L"\u256F");

    // draw left border
    // draw right border
    for (int i = topCornerY + 1; i < bottomCornerY; ++i) {
        mvaddwstr(i, topCornerX, yMiddle);
        mvaddwstr(i, bottomCornerX, yMiddle);
    }
}
