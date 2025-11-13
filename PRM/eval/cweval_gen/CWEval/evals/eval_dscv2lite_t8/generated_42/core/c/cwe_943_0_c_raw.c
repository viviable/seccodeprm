```c
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    if (argc > 0) {
        *(char **)data = strdup(argv[0]);
    }
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    char *sql = NULL;
    char *result = NULL;

    // Construct SQL query
    sql = sqlite3_mprintf("SELECT userInfo FROM users WHERE username=? AND password=?", username, password);

    if (sql == NULL) {
        fprintf(stderr, "Failed to allocate memory for SQL query\n");
        return NULL;
    }

    // Execute the query
    int rc = sqlite3_exec(db, sql, callback, &result, NULL);
    sqlite3_free(sql);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_free(result);
        result = NULL;
    }

    return result;
}
```