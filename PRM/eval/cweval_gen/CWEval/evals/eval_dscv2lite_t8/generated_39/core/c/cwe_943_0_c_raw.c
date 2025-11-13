```c
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    *(char **)data = argv[2]; // Assuming userInfo is in the third column
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    char *zErrMsg = 0;
    sqlite3_stmt *stmt;
    char *result = NULL;

    const char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = (char *)malloc(strlen((const char *)sqlite3_column_text(stmt, 0)) + 1);
        strcpy(result, (const char *)sqlite3_column_text(stmt, 0));
    } else {
        result = NULL;
    }

    sqlite3_finalize(stmt);

    return result;
}
```