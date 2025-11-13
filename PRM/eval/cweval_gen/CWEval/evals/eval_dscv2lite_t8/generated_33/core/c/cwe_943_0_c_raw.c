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
    char *sql = NULL;
    sqlite3_stmt *stmt = NULL;
    char *userInfo = NULL;

    // Construct the SQL query
    sql = (char *)malloc(256);
    snprintf(sql, 256, "SELECT userInfo FROM users WHERE username = ? AND password = ?", username, password);

    // Prepare the statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        free(sql);
        return NULL;
    }

    // Bind parameters
    if (sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "Bind error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        free(sql);
        return NULL;
    }

    // Execute the query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userInfo = (char *)malloc(256);
        strcpy(userInfo, (const char *)sqlite3_column_text(stmt, 0));
    } else {
        userInfo = NULL;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    free(sql);

    return userInfo;
}
```