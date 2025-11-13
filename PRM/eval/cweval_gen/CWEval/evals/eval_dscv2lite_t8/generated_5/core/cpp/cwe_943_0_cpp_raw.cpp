```cpp

using namespace std;
#include <sqlite3.h>

extern "C" {
    #include <sqlite3ext.h>
}

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        // Handle error
        sqlite3_finalize(stmt);
        return "";
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        std::string result(reinterpret_cast<const char*>(userInfo));
        sqlite3_finalize(stmt);
        return result;
    } else {
        sqlite3_finalize(stmt);
        return "";
    }
}
```