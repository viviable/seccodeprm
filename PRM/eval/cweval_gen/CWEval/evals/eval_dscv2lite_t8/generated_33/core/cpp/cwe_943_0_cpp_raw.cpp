```cpp

using namespace std;
#include <iostream>
#include <string>
#include <sqlite3.h>

extern "C" int sqlite3_open(const char *filename, sqlite3 **db);
extern "C" int sqlite3_close(sqlite3 *db);
extern "C" int sqlite3_exec(sqlite3 *db, const char *sql, int (*callback)(void*,int,char**,char**), void *, char **errmsg);

string getUser(sqlite3* db, const string& username, const string& password) {
    string query = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute the query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        if (userInfo) {
            string result(reinterpret_cast<const char*>(userInfo));
            sqlite3_finalize(stmt);
            return result;
        }
    }

    if (rc != SQLITE_DONE) {
        cerr << "Query failed: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return "";
}
```