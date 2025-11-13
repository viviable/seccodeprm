```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

char *get_user(sqlite3 *db, const char *username, const char *password) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    char *userInfo = NULL;

    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
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
        break; // Assuming we only expect one matching record
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        free(userInfo); // Free any allocated memory in case of error
        return NULL;
    }

    return userInfo;
}
```