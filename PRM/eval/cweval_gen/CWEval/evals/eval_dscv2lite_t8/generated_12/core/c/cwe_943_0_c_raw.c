```c
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    char **result = (char **)data;
    if (argc > 0) {
        *result = argv[0];
    }
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    char *sql = NULL;
    char *errmsg = NULL;
    char *userInfo = NULL;

    // Prepare SQL statement to check if the user exists with the given credentials
    sql = sqlite3_mprintf("SELECT userInfo FROM users WHERE username = ? AND password = ?", username, password);

    if (sql == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Execute the SQL query
    if (sqlite3_exec(db, sql, callback, &userInfo, &errmsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        sqlite3_free(sql);
        return NULL;
    }

    sqlite3_free(sql);

    return userInfo;
}
```