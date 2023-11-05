#include "tree_viewer.h"
#include "box.h"
#include "command_center.h"
#include "panel.h"
#include "window.h"

muh_panel *init_tree_viewer(void) {
    struct muh_box tree_viewer_box = {
        {0, 0},
        {TREE_VIEWER_WIDTH, COMMAND_CENTER_TOP - 1},
    };

    return new_panel(tree_viewer_box, "Tables");
}
