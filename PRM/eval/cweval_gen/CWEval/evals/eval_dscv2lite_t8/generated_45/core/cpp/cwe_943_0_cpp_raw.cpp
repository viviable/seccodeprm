```cpp

#include <iostream>
#include <string>
#include <sqlite3.h>

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt;
    std::string query = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    
    if (rc == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        sqlite3_finalize(stmt);
        return std::string(reinterpret_cast<const char*>(userInfo));
    } else if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return "";
    } else {
        std::cerr << "SQL step error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return "";
    }
}
```
This function uses SQLite's prepared statements to safely execute a query that retrieves user information based on the provided username and password. It handles errors gracefully and returns the user information if the user exists, otherwise it returns an empty string.