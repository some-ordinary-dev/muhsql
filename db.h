#ifndef MUH_DB
#define MUH_DB

#include <sqlite3.h>

typedef struct muh_db muh_db;

/* Fills the string array with the tables available in the DB */
int get_tables(muh_db *db, char ***tables);

void open_db(muh_db **db);
void close_db(muh_db *db);

#endif
