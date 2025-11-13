```c
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    if (argc > 0) {
        strcpy((char *)data, argv[0]);
        return 1; // Stop after first row
    }
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    sqlite3_stmt *stmt = NULL;
    char sql[256];
    char *result = NULL;

    snprintf(sql, sizeof(sql), "SELECT userInfo FROM users WHERE username = ? AND password = ?", username, password);

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        goto cleanup;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        result = (char *)malloc(strlen((const char *)sqlite3_column_text(stmt, 0)) + 1);
        if (result) {
            strcpy(result, (const char *)sqlite3_column_text(stmt, 0));
        } else {
            fprintf(stderr, "Memory allocation failed\n");
        }
        break;
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        fprintf(stderr, "Error executing query: %s\n", sqlite3_errmsg(db));
    }

cleanup:
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    return result;
}
```