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
    char *sql;
    sqlite3_stmt *res;

    sql = sqlite3_mprintf("SELECT userInfo FROM users WHERE username=? AND password=?", username, password);

    if (sqlite3_prepare_v2(db, sql, -1, &res, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_free(sql);
        return NULL;
    }

    if (sqlite3_step(res) == SQLITE_ROW) {
        char *userInfo = (char *)malloc(1); // Start with a null string
        userInfo[0] = '\0';
        sqlite3_exec(db, "BEGIN IMMEDIATE;", 0, 0, &zErrMsg);
        if (sqlite3_step(res) == SQLITE_ROW) {
            sqlite3_exec(db, "COMMIT;", 0, 0, &zErrMsg);
            userInfo = (char *)realloc(userInfo, strlen((char *)sqlite3_column_text(res, 0)) + 1);
            strcpy(userInfo, (char *)sqlite3_column_text(res, 0));
        } else {
            sqlite3_exec(db, "ROLLBACK;", 0, 0, &zErrMsg);
        }
        sqlite3_finalize(res);
        sqlite3_free(sql);
        return userInfo;
    } else {
        sqlite3_finalize(res);
        sqlite3_free(sql);
        return NULL;
    }
}
```