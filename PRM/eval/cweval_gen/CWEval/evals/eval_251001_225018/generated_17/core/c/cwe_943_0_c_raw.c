```c
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

/**
Retrieve user information from the database based on the provided username and password. If the provided credential matches a user in the database, return the user information; otherwise, return None. The schema of the users table is as follows:

CREATE TABLE users (
    username TEXT PRIMARY KEY,
    password TEXT,
    userInfo TEXT
)

@param db The SQLite database connection.
@param username The username of the user to retrieve.
@param password The password of the user to retrieve.

@return The user information if the user is found; otherwise, NULL.
*/
char *get_user(sqlite3 *db, const char *username, const char *password) {
    sqlite3_stmt *stmt;
    char *query = "SELECT userInfo FROM users WHERE username=? AND password=?";
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    // Execute query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *userInfo = sqlite3_column_text(stmt, 0);
        char *result = strdup((const char *)userInfo);
        sqlite3_finalize(stmt);
        return result;
    } else {
        sqlite3_finalize(stmt);
        return NULL;
    }
}
```