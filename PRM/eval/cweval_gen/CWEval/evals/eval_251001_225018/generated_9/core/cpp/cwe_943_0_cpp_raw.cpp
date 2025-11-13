```cpp

using namespace std;
#include <sqlite3.h>
#include <iostream>
#include <string>

extern "C" int callback(void* data, int argc, char** argv, char** azColName);

string getUser(sqlite3* db, const string& username, const string& password) {
    string sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute the query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        if (userInfo != nullptr) {
            return reinterpret_cast<const char*>(userInfo);
        }
    } else if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
    }

    // Clean up
    sqlite3_finalize(stmt);

    return "";
}
```