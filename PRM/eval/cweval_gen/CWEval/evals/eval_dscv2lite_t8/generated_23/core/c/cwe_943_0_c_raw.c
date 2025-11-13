```c
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    char **result = (char **)data;
    *result = strdup(argv[2]); // Assuming userInfo is in the third column
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    sqlite3_stmt *stmt;
    char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    char *userInfo = NULL;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        userInfo = (char *)sqlite3_column_text(stmt, 0);
    } else if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error querying database: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return userInfo;
}
```