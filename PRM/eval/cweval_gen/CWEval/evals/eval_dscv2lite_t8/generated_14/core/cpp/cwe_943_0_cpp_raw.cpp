```cpp

using namespace std;
#include <sqlite3.h>
#include <iostream>
#include <string>

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    std::string query = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        // Handle error
        sqlite3_finalize(stmt);
        return "";
    }
    
    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    
    // Execute query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        if (userInfo != nullptr) {
            sqlite3_finalize(stmt);
            return reinterpret_cast<const char*>(userInfo);
        }
    }
    
    // Check for errors other than no row found
    if (rc != SQLITE_DONE && rc != SQLITE_ERROR) {
        // Handle error
        sqlite3_finalize(stmt);
        return "";
    }
    
    // No matching user found
    sqlite3_finalize(stmt);
    return "";
}
```