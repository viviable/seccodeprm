```c
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

char *get_user(sqlite3 *db, const char *username, const char *password) {
    sqlite3_stmt *stmt;
    char *query = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    int rc;

    // Prepare the SQL statement
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    // Bind parameters to the SQL statement
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    // Execute the SQL statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Retrieve the user information
        const unsigned char *userInfo = sqlite3_column_text(stmt, 0);
        char *result = strdup((const char *)userInfo);
        sqlite3_finalize(stmt);
        return result;
    } else if (rc == SQLITE_DONE) {
        // No matching user found
        sqlite3_finalize(stmt);
        return NULL;
    } else {
        // Error during execution
        fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }
}
```