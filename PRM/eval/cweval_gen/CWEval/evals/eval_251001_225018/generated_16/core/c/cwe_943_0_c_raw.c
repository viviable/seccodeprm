```c
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    char **userInfo = (char **)data;
    if (argc > 0) {
        *userInfo = strdup(argv[0]);
    }
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    sqlite3_stmt *stmt;
    char *userInfo = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        userInfo = (char *)sqlite3_column_text(stmt, 0);
        break;
    }

    sqlite3_finalize(stmt);

    return userInfo;
}
```