```cpp
#include <sqlite3.h>
#include <iostream>
#include <string>

extern "C" {
    #include <sqlite3ext.h>
}

static int getUserCallback(void* data, int argc, char** argv, char** azColName) {
    if (argc > 2 && argv[1] != nullptr && argv[2] != nullptr) {
        *static_cast<std::string*>(data) = argv[2];
        return SQLITE_DONE;
    }
    return SQLITE_OK;
}

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    std::string userInfo = "";
    std::string sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return userInfo;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        getUserCallback(&userInfo, sqlite3_column_count(stmt), sqlite3_column_text(stmt, i), nullptr);
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return userInfo;
}
```
This solution includes error handling for SQLite operations and uses prepared statements to prevent SQL injection. It also demonstrates how to use `sqlite3_step` to iterate through the results and a custom callback function to process each row returned by the query.