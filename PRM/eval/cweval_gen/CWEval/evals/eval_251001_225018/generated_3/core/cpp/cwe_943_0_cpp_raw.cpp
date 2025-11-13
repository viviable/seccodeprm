```cpp

using namespace std;
#include <sqlite3.h>
#include <string>

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    std::string userInfo;

    // Prepare SQL statement to select user info based on username and password
    int rc = sqlite3_prepare_v2(db, "SELECT userInfo FROM users WHERE username = ? AND password = ?", -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return "";
    }

    // Bind parameters to the SQL statement
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute the query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        userInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    // Check for errors during execution
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return "";
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    return userInfo;
}
```