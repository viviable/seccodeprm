```cpp

using namespace std;

#include <iostream>
#include <memory>
#include <string>
#include <sqlite3.h>

extern "C" int sqlite3_open(const char *filename, sqlite3 **ppDb);
extern "C" int sqlite3_close(sqlite3 *db);
extern "C" int sqlite3_exec(sqlite3 *db, const char *sql, int (*callback)(void*,int,char**,char**), void *arg, char **errmsg);

string getUser(sqlite3* db, const string& username, const string& password) {
    string query = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    unique_ptr<char[]> zErrMsg(nullptr);
    int rc;
    string result;

    rc = sqlite3_exec(db, query.c_str(), [](void* data, int argc, char** argv, char** azColName) -> int {
        static_cast<string*>(data)->assign(argv[0]);
        return 0;
    }, &result, &zErrMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg.get() << endl;
        sqlite3_free(zErrMsg.get());
    }

    return result;
}
```