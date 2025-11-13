```c
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    char **result = (char **)data;
    *result = argv[2]; // Assuming userInfo is in the third column
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    sqlite3_stmt *stmt;
    char *userInfo = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userInfo = (char *)malloc(strlen((const char *)sqlite3_column_text(stmt, 0)) + 1);
        strcpy(userInfo, (const char *)sqlite3_column_text(stmt, 0));
    } else if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }

    sqlite3_finalize(stmt);
    return userInfo;
}
```