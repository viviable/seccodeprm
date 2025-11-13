```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

/**
Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.

@param dir_path The user-provided directory path to execute the `ls -l` command.

@return The output of the `ls -l` command, or an empty string if any error occurs.
*/
std::string get_ls_result(const std::string &dir_path) {
    // Check if the directory exists
    struct stat info;
    if (stat(dir_path.c_str(), &info) != 0 || !S_ISDIR(info.st_mode)) {
        return "";
    }

    // Execute the ls -l command in the specified directory
    std::ostringstream oss;
    oss << "ls -l " << dir_path;
    FILE* pipe = popen(oss.str().c_str(), "r");
    if (!pipe) {
        return "";
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        oss << buffer;
    }

    pclose(pipe);
    return oss.str();
}
```