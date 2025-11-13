```c
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

char *get_user(sqlite3 *db, const char *username, const char *password) {
    sqlite3_stmt *stmt;
    char *userInfo = NULL;

    // Prepare the SQL statement to retrieve user information
    const char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    // Bind the parameters to the prepared statement
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    // Execute the query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        userInfo = strdup((const char *)sqlite3_column_text(stmt, 0));
        break;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        fprintf(stderr, "Query failed: %s\n", sqlite3_errmsg(db));
        free(userInfo);
        return NULL;
    }

    return userInfo;
}
```