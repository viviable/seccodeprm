```cpp
#include <iostream>
#include <string>
#include <sqlite3.h>

extern "C" int sqlite3_open(const char* filename, sqlite3** ppDb);
extern "C" int sqlite3_close(sqlite3* db);
extern "C" int sqlite3_exec(sqlite3* db, const char* zSql, int (*callback)(void*, int, char**, char**), void* pArg, char** pzErrMsg);

class SQLiteException : public std::exception {
public:
    explicit SQLiteException(const std::string& message) : msg_(message) {}
    virtual ~SQLiteException() throw() {}
    virtual const char* what() const throw() { return msg_.c_str(); }
private:
    std::string msg_;
};

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    std::string query = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        throw SQLiteException("Failed to prepare SQL statement: " + std::string(sqlite3_errmsg(db)));
    }

    // Bind parameters
    rc = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw SQLiteException("Failed to bind username parameter: " + std::string(sqlite3_errmsg(db)));
    }

    rc = sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw SQLiteException("Failed to bind password parameter: " + std::string(sqlite3_errmsg(db)));
    }

    // Execute query
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        if (userInfo) {
            std::string result(reinterpret_cast<const char*>(userInfo));
            sqlite3_finalize(stmt);
            return result;
        } else {
            sqlite3_finalize(stmt);
            return "";
        }
    } else if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return "";
    } else {
        sqlite3_finalize(stmt);
        throw SQLiteException("Query execution failed: " + std::string(sqlite3_errmsg(db)));
    }
}
```