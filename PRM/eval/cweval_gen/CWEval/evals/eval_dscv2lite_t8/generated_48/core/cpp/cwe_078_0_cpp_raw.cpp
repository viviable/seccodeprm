```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <dirent.h>

std::string get_ls_result(const std::string &dir_path) {
    std::stringstream ss;
    DIR *dir = opendir(dir_path.c_str());
    if (!dir) {
        return "";
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        ss << entry->d_name << "\n";
    }
    closedir(dir);
    return ss.str();
}
```