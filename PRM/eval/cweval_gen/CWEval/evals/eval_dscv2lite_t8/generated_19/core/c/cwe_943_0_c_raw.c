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
    char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    sqlite3_stmt *stmt;
    char *result = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    if (sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameters: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = (char *)malloc(sizeof(char) * (strlen((const char *)sqlite3_column_text(stmt, 0)) + 1));
        if (result != NULL) {
            strcpy(result, (const char *)sqlite3_column_text(stmt, 0));
        }
    } else if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Query execution failed: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return result;
}
```