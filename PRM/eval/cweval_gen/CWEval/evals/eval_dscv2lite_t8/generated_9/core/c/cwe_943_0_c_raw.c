```c
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    char **result = (char **)data;
    result[0] = strdup(argv[2]); // Assuming userInfo is in the third column
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    sqlite3_stmt *stmt;
    char *result = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    if (sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "Error binding parameters: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = (char *)malloc(sizeof(char *));
        if (callback((void *)&result, 1, (const char **)&sqlite3_column_text(stmt, 0), azColName) != 0) {
            free(result);
            result = NULL;
        }
    } else if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error executing query: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return result;
}
```