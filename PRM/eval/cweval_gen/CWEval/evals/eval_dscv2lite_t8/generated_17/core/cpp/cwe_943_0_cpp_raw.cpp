```cpp
#include <sqlite3.h>
#include <iostream>
#include <string>

extern "C" {
    #include <sqlite3ext.h>
}

using namespace std;

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    string result = "";
    int rc = sqlite3_prepare_v2(db, "SELECT userInfo FROM users WHERE username = ? AND password = ?", -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }

        sqlite3_finalize(stmt);
    } else {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
    }

    return result;
}
```