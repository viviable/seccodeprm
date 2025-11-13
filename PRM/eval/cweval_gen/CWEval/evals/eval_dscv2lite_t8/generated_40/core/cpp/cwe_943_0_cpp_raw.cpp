```cpp

using namespace std;

#include <sqlite3.h>

string getUser(sqlite3* db, const string& username, const string& password) {
    sqlite3_stmt* stmt;
    int rc;
    string userInfo = "";

    // Prepare the SQL statement
    const char* sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return userInfo;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute the query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        userInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    // Check for errors
    if (rc != SQLITE_DONE) {
        cerr << "Error executing query: " << sqlite3_errmsg(db) << endl;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    return userInfo;
}
```