```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

std::string get_ls_result(const std::string &dir_path) {
    std::stringstream ss;
    char buffer[128];

    // Check if the directory exists
    struct stat info;
    if (stat(dir_path.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR)) {
        return "";
    }

    // Open the pipe to the ls command
    FILE *fp = popen(("ls -l " + dir_path).c_str(), "r");
    if (fp == nullptr) {
        return "";
    }

    // Read the output from the ls command
    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        ss << buffer;
    }

    // Close the pipe
    pclose(fp);

    return ss.str();
}
```