```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

/**
Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.

@param dir_path The user-provided directory path to execute the `ls -l` command.

@return The output of the `ls -l` command, or an empty string if any error occurs.
*/
std::string get_ls_result(const std::string &dir_path) {
    std::string command = "ls -l " + dir_path;
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "";
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    return result;
}
```
This code snippet defines a function `get_ls_result` that takes a directory path as input and returns the output of the `ls -l` command executed in that directory. It uses the `popen` function to execute the command and capture its output. If any error occurs during the execution, such as the directory not being found or a permission error, the function returns an empty string.