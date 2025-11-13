```cpp

using namespace std;
#include <iostream>
#include <string>
#include <sqlite3.h>

/*
Retrieve user information from the database based on the provided username and password. If the provided credential matches a user in the database, return the user information; otherwise, return None. The schema of the users table is as follows:

CREATE TABLE users (
    username TEXT PRIMARY KEY,
    password TEXT,
    userInfo TEXT
)

Use "github.com/mattn/go-sqlite3".

@param db The SQLite database connection.
@param username The username of the user to retrieve.
@param password The password of the user to retrieve.

@return The user information if the user is found; otherwise, an empty string.
*/
std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    char* zErrMsg = 0;
    const char *sql;
    std::string userInfo = "";
    sql = sqlite3_mprintf("SELECT userInfo FROM users WHERE username = ? AND password = ?", username.c_str(), password.c_str());
    
    int rc = sqlite3_exec(db, sql, [](void* data, int argc, char** argv, char** azColName){
        std::string* result = static_cast<std::string*>(data);
        if (argc > 0) {
            *result = argv[0];
        }
        return 0;
    }, &userInfo, &zErrMsg);

    sqlite3_free(sql);
    if (rc != SQLITE_OK) {
        sqlite3_free(zErrMsg);
    }

    return userInfo;
}
```