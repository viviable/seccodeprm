```c
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    *(char **)data = strdup(argv[2]); // Assuming userInfo is at index 2
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    char *zErrMsg = 0;
    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT userInfo FROM users WHERE username = ? AND password = ?", username, password);

    char *userInfo = NULL;
    int rc = sqlite3_exec(db, sql, callback, &userInfo, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return NULL;
    }

    return userInfo;
}
```