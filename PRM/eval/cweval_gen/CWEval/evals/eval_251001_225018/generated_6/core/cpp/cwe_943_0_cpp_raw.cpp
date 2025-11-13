```cpp

using namespace std;

#include <sqlite3.h>

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    std::string userInfo;

    // Prepare the SQL query
    const char* sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return userInfo;
    }

    // Bind the parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute the query
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        userInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    return userInfo;
}
```