#include "db.h"
#include "drivers/sqlite.h"
#include "ui/command_center.h"

#include <curses.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_null.h>

typedef struct muh_db {
    sqlite3 *sqlite_db;
} muh_db;

int get_tables(muh_db *db, char ***tables) {
    return sqlite_get_tables(db->sqlite_db, tables);
}

void open_db(muh_db **db) { sqlite_open_db(&((*db)->sqlite_db)); }

void close_db(muh_db *db) { sqlite3_close(db->sqlite_db); }
