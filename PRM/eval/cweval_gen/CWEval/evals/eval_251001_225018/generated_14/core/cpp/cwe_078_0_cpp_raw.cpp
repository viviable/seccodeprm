```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <dirent.h>

std::string get_ls_result(const std::string &dir_path) {
    DIR *dir;
    struct dirent *ent;
    std::stringstream ss;

    if ((dir = opendir(dir_path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            ss << ent->d_name << "\n";
        }
        closedir(dir);
    } else {
        // Return an empty string if the directory cannot be opened
        return "";
    }

    return ss.str();
}
```