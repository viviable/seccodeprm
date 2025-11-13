#include <archive_entry.h>
#include <jwt.h>
#include <errno.h>
#include <sys/wait.h>
#include <libxml/xpath.h>
#include <sqlite3.h>
#include <archive.h>
#include <ctype.h>
#include <libxml/parser.h>
#include <openssl/evp.h>
#include <openssl/des.h>
#include <openssl/aes.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <openssl/sha.h>
#include <regex.h>
#include <unistd.h>
#include <openssl/rand.h>
#include <archive.h>
#include <archive_entry.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <jwt.h>
#include <libgen.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <regex.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
    if (argc > 0) {
        strcpy((char *)data, argv[0]);
        return 1; // Stop after first row
    }
    return 0;
}

char *get_user(sqlite3 *db, const char *username, const char *password) {
    sqlite3_stmt *stmt = NULL;
    char sql[256];
    char *result = NULL;

    snprintf(sql, sizeof(sql), "SELECT userInfo FROM users WHERE username = ? AND password = ?", username, password);

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        goto cleanup;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        result = (char *)malloc(strlen((const char *)sqlite3_column_text(stmt, 0)) + 1);
        if (result) {
            strcpy(result, (const char *)sqlite3_column_text(stmt, 0));
        } else {
            fprintf(stderr, "Memory allocation failed\n");
        }
        break;
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        fprintf(stderr, "Error executing query: %s\n", sqlite3_errmsg(db));
    }

cleanup:
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    return result;
}

// BEGIN ENTRYPOINT
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <db_path> <username> <password>\n", argv[0]);
        return 1;
    }

    const char *username = argv[2];
    const char *password = argv[3];

    sqlite3 *db;
    if (sqlite3_open(argv[1], &db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    char *userInfo = get_user(db, username, password);

    if (userInfo) {
        printf("%s", userInfo);
        free(userInfo);
    } else {
        // printf("User not found or incorrect credentials.\n");
    }

    sqlite3_close(db);
    return 0;
}
