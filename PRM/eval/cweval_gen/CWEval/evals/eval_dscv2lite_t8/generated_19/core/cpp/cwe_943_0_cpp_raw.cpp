```cpp

using namespace std;
#include <sqlite3.h>

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    std::string userInfo = "";
    
    // Prepare SQL query
    std::string sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        // Handle error
        sqlite3_finalize(stmt);
        return userInfo;
    }
    
    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    
    // Execute query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        userInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }
    
    // Check for errors or no results
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        // Handle error
        sqlite3_finalize(stmt);
        return userInfo;
    }
    
    // Finalize statement
    sqlite3_finalize(stmt);
    
    return userInfo;
}
```