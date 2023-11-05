#ifndef MUH_COMMANDS
#define MUH_COMMANDS

#include "panel.h"
#include "window.h"

#define MAX_MESSAGES 6
#define COMMAND_CENTER_TOP (MAX_Y - MAX_MESSAGES - 1)

muh_panel *init_cmd_center(void);
void log_msg(const char *msg);

#endif
