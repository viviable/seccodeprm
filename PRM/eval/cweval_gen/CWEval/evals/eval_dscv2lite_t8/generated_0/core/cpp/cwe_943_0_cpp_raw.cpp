```cpp

using namespace std;
#include <sqlite3.h>
#include <iostream>
#include <string>

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

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        std::string result(userInfo ? reinterpret_cast<const char*>(userInfo) : "");
        sqlite3_finalize(stmt);
        return result;
    } else {
        sqlite3_finalize(stmt);
        return "";
    }
}
```
This code snippet includes necessary headers and defines the `getUser` function that retrieves user information from an SQLite database based on the provided username and password. It uses prepared statements to safely execute the SQL query and bind parameters to prevent SQL injection attacks. If a matching user is found, it returns the user's information; otherwise, it returns an empty string.