```c
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    if (argc > 0 && argv[0] != NULL) {
        *(char **)data = strdup(argv[0]);
    }
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    char *sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    sqlite3_stmt *stmt;
    char *userInfo = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            userInfo = (char *)malloc(1); // Initialize to hold at least one byte
            userInfo[0] = '\0';
            sqlite3_exec(stmt, "SELECT userInfo FROM users WHERE username=? AND password=?", callback, &userInfo, NULL);
        }

        sqlite3_finalize(stmt);
    }

    return userInfo;
}
```