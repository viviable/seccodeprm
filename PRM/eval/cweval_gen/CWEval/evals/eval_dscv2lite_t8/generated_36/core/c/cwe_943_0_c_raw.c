```c
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

char *get_user(sqlite3 *db, const char *username, const char *password) {
    sqlite3_stmt *stmt;
    int rc;
    char *userInfo = NULL;

    // Prepare the SQL statement
    const char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    // Bind parameters to the prepared statement
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    // Execute the statement
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        userInfo = strdup((const char *)sqlite3_column_text(stmt, 0));
        break;
    }

    // Check for errors
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        fprintf(stderr, "Error executing query: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    return userInfo;
}
```