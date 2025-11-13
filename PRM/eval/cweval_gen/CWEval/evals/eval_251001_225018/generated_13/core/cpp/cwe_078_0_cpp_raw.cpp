```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

std::string get_ls_result(const std::string &dir_path) {
    std::string result;
    DIR *dir = opendir(dir_path.c_str());
    if (!dir) {
        return result; // Return empty string if directory cannot be opened
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::ostringstream oss;
        oss << entry->d_name << "\n";
        result += oss.str();
    }

    closedir(dir);
    return result;
}
```