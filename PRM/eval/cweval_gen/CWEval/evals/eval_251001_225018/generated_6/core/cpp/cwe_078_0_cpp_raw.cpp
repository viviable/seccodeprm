```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <dirent.h>

std::string get_ls_result(const std::string &dir_path) {
    DIR *dir;
    struct dirent *ent;
    std::ostringstream oss;

    if ((dir = opendir(dir_path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            oss << ent->d_name << "\n";
        }
        closedir(dir);
    } else {
        return "";
    }

    return oss.str();
}
```