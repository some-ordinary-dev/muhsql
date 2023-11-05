#include <curses.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_null.h>

#include "../ui/command_center.h"
#include "sqlite.h"

static const char *SEED_DATA =
    "CREATE TABLE table1(Id INTEGER PRIMARY KEY, Name TEXT);"
    "CREATE TABLE test_tbl(Id INTEGER PRIMARY KEY, Name TEXT);"
    "CREATE TABLE table2(Id INTEGER PRIMARY KEY, Name TEXT);";

int seed_db(sqlite3 *db) {
    char *err_msg;
    int rc = sqlite3_exec(db, SEED_DATA, NULL, NULL, &err_msg);
    if (err_msg != NULL) {
        log_msg(err_msg);
        sqlite3_free(err_msg);
        return 1;
    }
    return 0;
}

int print_version(sqlite3 *db) {
    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, 0);

    if (rc != SQLITE_OK) {
        char err_msg[100];
        sprintf(err_msg, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        log_msg(err_msg);

        sqlite3_finalize(res);
        sqlite3_close(db);

        db = NULL;
        return 1;
    }

    rc = sqlite3_step(res);

    if (rc == SQLITE_ROW) {
        char sqlite_version_msg[100];
        sprintf(sqlite_version_msg, "Using SQLite version %s",
                sqlite3_column_text(res, 0));
        log_msg(sqlite_version_msg);
    }

    sqlite3_finalize(res);
    return 0;
}

int sqlite_open_db(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);

    if (rc != SQLITE_OK) {
        char err_msg[100];
        sprintf(err_msg, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        log_msg(err_msg);

        sqlite3_close(*db);
        return rc;
    }

    rc = print_version(*db);

    rc = seed_db(*db);
    if (rc) {
        sqlite3_close(*db);
        return rc;
    }

    return 0;
}

int sqlite_get_tables(sqlite3 *db, char ***tables) {
    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(
        db, "SELECT name FROM sqlite_schema WHERE type = 'table';", -1, &res,
        0);

    if (rc != SQLITE_OK) {
        char err_msg[100];
        sprintf(err_msg, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        log_msg(err_msg);
        sqlite3_finalize(res);
        return 0;
    }

    int rowCount = 0;
    while ((rc = sqlite3_step(res)) == SQLITE_ROW) {
        rowCount++;
    }

    sqlite3_reset(res);

    // size array to hold tables
    *tables = malloc(sizeof(char *) * rowCount);

    for (int i = 0; (rc = sqlite3_step(res)) == SQLITE_ROW; ++i) {
        const unsigned char *row = sqlite3_column_text(res, 0);

        size_t len = strlen((char *)row) + 1;
        char *copy = malloc(sizeof(char) * len);
        memset(copy, '\0', len);
        strncpy(copy, (char *)row, len - 1);

        (*tables)[i] = copy;
    }

    return rowCount;
}
