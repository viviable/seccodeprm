```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    strcpy((char *)data, argv[2]);
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    sqlite3_stmt *stmt;
    char sql[256];
    char *userInfo = (char *)malloc(1); // Allocate initial memory
    userInfo[0] = '\0';

    snprintf(sql, sizeof(sql), "SELECT userInfo FROM users WHERE username=? AND password=?", username, password);

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        free(userInfo);
        return NULL;
    }

    rc = sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind username: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        free(userInfo);
        return NULL;
    }

    rc = sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind password: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        free(userInfo);
        return NULL;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        userInfo = realloc(userInfo, strlen((char *)sqlite3_column_text(stmt, 0)) + 1);
        strcpy(userInfo, (char *)sqlite3_column_text(stmt, 0));
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Query failed: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return userInfo;
}
```