```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/**
Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.

@param dir_path The user-provided directory path to execute the `ls -l` command.

@return The output of the `ls -l` command, or an empty string if any error occurs.
*/
std::string get_ls_result(const std::string &dir_path) {
    std::string command = "ls -l " + dir_path;
    std::array<char, 128> buffer;
    std::string result;

    // Open a pipe to the command's output
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return ""; // Return empty string on failure

    try {
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
    } catch (...) {
        pclose(pipe);
        throw; // Re-throw the exception
    }

    pclose(pipe);
    return result;
}
```