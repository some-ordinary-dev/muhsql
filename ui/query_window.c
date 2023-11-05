#include "query_window.h"
#include "box.h"
#include "command_center.h"
#include "panel.h"
#include "tree_viewer.h"
#include "window.h"

muh_panel *init_query_window(void) {
    struct muh_box query_window_box = {
        {TREE_VIEWER_WIDTH + 1, 0},
        {MAX_X, COMMAND_CENTER_TOP - 1},
    };

    return new_panel(query_window_box, "Query Window");
}
