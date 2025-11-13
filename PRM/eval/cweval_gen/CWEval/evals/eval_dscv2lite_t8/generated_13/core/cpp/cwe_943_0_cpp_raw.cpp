```cpp

#include <iostream>
#include <memory>
#include <string>
#include <sqlite3.h>

extern "C" int sqlite3_exec(
  sqlite3*,               /* An open database */
  const char *sql,        /* SQL to be evaluated */
  int (*callback)(void*,int,char**,char**), /* Callback function */
  void *,                 /* 1st argument to callback */
  char **errmsg           /* Error msg written here */
);

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    std::string query = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        if (userInfo) {
            std::string result(reinterpret_cast<const char*>(userInfo));
            sqlite3_finalize(stmt);
            return result;
        }
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return "";
}
```