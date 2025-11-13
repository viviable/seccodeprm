```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>

std::string get_ls_result(const std::string &dir_path) {
    std::string result;
    struct stat info;

    // Check if the directory exists
    if (stat(dir_path.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR)) {
        return "";
    }

    // Open the pipe for the ls -l command
    FILE *pipe = popen(("ls -l " + dir_path).c_str(), "r");
    if (!pipe) {
        return "";
    }

    // Read the output from the pipe
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    // Close the pipe
    pclose(pipe);

    return result;
}
```