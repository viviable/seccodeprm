```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    *(char **)data = strdup(argv[2]);
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    sqlite3_stmt *stmt;
    char *userInfo = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userInfo = (char *)malloc(1); // Allocate initial memory
        userInfo[0] = '\0'; // Null-terminate the string

        int len = sqlite3_column_bytes(stmt, 0);
        userInfo = realloc(userInfo, len + 1); // Resize memory to fit the data
        strcpy(userInfo, (const char *)sqlite3_column_text(stmt, 0)); // Copy data
    } else {
        sqlite3_finalize(stmt);
        return NULL;
    }

    sqlite3_finalize(stmt);
    return userInfo;
}
```