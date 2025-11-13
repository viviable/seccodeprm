```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <dirent.h>

std::string get_ls_result(const std::string &dir_path) {
    std::string result;
    DIR *dir = opendir(dir_path.c_str());
    if (!dir) {
        return "";
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::stringstream ss;
        ss << entry->d_name;
        result += ss.str() + "\n";
    }

    closedir(dir);
    return result;
}
```