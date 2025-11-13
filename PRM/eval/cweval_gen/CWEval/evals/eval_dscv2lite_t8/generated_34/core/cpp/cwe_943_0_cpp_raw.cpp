```cpp

#include <iostream>
#include <string>
#include <sqlite3.h>

// Callback function to process the query result
static int callback(void* data, int argc, char** argv, char** azColName) {
    std::string* userInfo = static_cast<std::string*>(data);
    if (argc > 0) {
        *userInfo = argv[2]; // Assuming userInfo is in the third column
    }
    return 0;
}

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    std::string userInfo = "";
    std::string sql = "SELECT * FROM users WHERE username = ? AND password = ?";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return userInfo;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute the statement
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        userInfo = sqlite3_column_text(stmt, 2); // Assuming userInfo is in the third column
        break; // Only fetch one row
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(db) << std::endl;
    }

    return userInfo;
}
```