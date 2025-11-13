```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

/**
Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.

@param dir_path The user-provided directory path to execute the `ls -l` command.

@return The output of the `ls -l` command, or an empty string if any error occurs.
*/
std::string get_ls_result(const std::string &dir_path) {
    std::stringstream cmd;
    cmd << "ls -l " << dir_path;

    FILE* pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) return "";

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, 128, pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}
```