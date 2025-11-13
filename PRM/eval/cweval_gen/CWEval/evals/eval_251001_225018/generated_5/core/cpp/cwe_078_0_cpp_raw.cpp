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
        return result; // Return empty string if directory does not exist or is not a directory
    }

    // Construct the command
    std::stringstream cmd;
    cmd << "ls -l " << dir_path;

    // Open a pipe to capture the output of the command
    FILE* pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) {
        return result; // Return empty string if unable to open pipe
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe); // Close the pipe

    return result;
}
```