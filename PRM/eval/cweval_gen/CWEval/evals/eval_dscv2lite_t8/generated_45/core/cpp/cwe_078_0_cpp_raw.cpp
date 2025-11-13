```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <dirent.h>

std::string get_ls_result(const std::string &dir_path) {
    std::string result;
    DIR *dir = opendir(dir_path.c_str());
    if (dir == nullptr) {
        return result; // Return empty string on error
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::ostringstream oss;
        oss << entry->d_name;
        result += oss.str() + "\n";
    }

    closedir(dir);
    return result;
}
```