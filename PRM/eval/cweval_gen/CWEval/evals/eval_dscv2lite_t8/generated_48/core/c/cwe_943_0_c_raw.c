```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    *(char **)data = argv[2]; // Assuming userInfo is the third column
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    char *zErrMsg = 0;
    char *sql = sqlite3_mprintf("SELECT userInfo FROM users WHERE username = ? AND password = ?", username, password);
    char *result = NULL;

    if (sqlite3_exec(db, sql, callback, &result, &zErrMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sqlite3_free(sql);
    return result;
}
```