```c
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    char **result = (char **)data;
    if (argc > 0) {
        *result = strdup(argv[0]);
    }
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    char *userInfo = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userInfo = (char *)sqlite3_column_text(stmt, 0);
    } else if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error while stepping through query results: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return userInfo;
}
```