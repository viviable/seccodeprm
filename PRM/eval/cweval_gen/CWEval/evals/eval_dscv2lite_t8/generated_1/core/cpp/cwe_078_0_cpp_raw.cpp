```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

std::string get_ls_result(const std::string &dir_path) {
    std::stringstream ss;
    struct stat buffer;

    // Check if the directory exists
    if (stat(dir_path.c_str(), &buffer) != 0) {
        return "";
    }

    // Check if the path is indeed a directory
    if (!S_ISDIR(buffer.st_mode)) {
        return "";
    }

    // Execute the ls -l command using popen
    FILE* pipe = popen(("ls -l " + dir_path).c_str(), "r");
    if (!pipe) {
        return "";
    }

    char buffer2[128];
    while (fgets(buffer2, sizeof(buffer2)-1, pipe) != nullptr) {
        ss << buffer2;
    }

    pclose(pipe);
    return ss.str();
}
```