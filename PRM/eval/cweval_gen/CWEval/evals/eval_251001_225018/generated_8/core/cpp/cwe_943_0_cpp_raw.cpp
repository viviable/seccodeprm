```cpp

using namespace std;
#include <sqlite3.h>

extern "C" {
    #include <sqlite3ext.h>
}

static int callback(void* data, int argc, char** argv, char** azColName) {
    *(static_cast<std::string*>(data)) = argv[2]; // Assuming userInfo is in the third column
    return 0;
}

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    std::string sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return ""; // Return empty string on SQL error
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    std::string result;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        return ""; // Return empty string on query error
    }

    return result;
}
```
This code snippet includes the necessary SQLite library and defines the `getUser` function that queries the database for user information based on the provided username and password. It uses prepared statements to prevent SQL injection attacks. The function returns the user information if a match is found, otherwise, it returns an empty string.