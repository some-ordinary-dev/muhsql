#ifndef MUH_SQLITE_DRIVER
#define MUH_SQLITE_DRIVER

#include <sqlite3.h>

int sqlite_open_db(sqlite3 **db);

int sqlite_get_tables(sqlite3 *db, char ***tables);

#endif
