```cpp
#include <iostream>
#include <string>
#include <sqlite3.h>

extern "C" {
    int callback(void* data, int argc, char** argv, char** azColName);
}

int callback(void* data, int argc, char** argv, char** azColName) {
    // Assuming we only expect one column named 'userInfo'
    if (argc > 0 && azColName[0] != nullptr && argv[0] != nullptr) {
        *static_cast<std::string*>(data) = argv[0];
    }
    return 0;
}

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    std::string sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    std::string result;
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Extract the userInfo column
        result = static_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else if (rc != SQLITE_DONE) {
        std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return result;
}
```