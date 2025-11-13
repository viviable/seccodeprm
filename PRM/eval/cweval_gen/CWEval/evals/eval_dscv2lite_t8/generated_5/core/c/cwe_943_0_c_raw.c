```c
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    *(char **)data = argv[2]; // Assuming userInfo is the third column
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    char *userInfo = NULL;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        userInfo = (char *)malloc(strlen((const char *)sqlite3_column_text(stmt, 0)) + 1);
        if (userInfo) {
            strcpy(userInfo, (const char *)sqlite3_column_text(stmt, 0));
        }
    }

    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error executing query: %s\n", sqlite3_errmsg(db));
        free(userInfo);
        return NULL;
    }

    return userInfo;
}
```