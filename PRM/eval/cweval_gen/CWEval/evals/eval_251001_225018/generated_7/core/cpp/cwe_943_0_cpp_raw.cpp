```cpp

using namespace std;
#include <sqlite3.h>
#include <string>

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    std::string userInfo = "";
    
    // Prepare the SQL statement
    std::string sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return userInfo; // Return empty string on error
    }
    
    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    
    // Execute the query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        userInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }
    
    // Finalize the statement
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        return ""; // Return empty string on error
    }
    
    return userInfo;
}
```
This code snippet defines a function `getUser` that retrieves user information from an SQLite database based on the provided username and password. It uses the `sqlite3` library to prepare and execute a SQL query. The function returns the user information if the credentials match a user in the database; otherwise, it returns an empty string.