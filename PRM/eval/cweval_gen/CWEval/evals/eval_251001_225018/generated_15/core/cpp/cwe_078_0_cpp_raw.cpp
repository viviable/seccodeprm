```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

/**
Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.

@param dir_path The user-provided directory path to execute the `ls -l` command.

@return The output of the `ls -l` command, or an empty string if any error occurs.
*/
std::string get_ls_result(const std::string &dir_path) {
    try {
        // Construct the command
        std::string command = "ls -l " + dir_path;

        // Use popen to execute the command
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }

        // Read the output a line at a time
        std::stringstream buffer;
        char buffer2[128];
        while (fgets(buffer2, sizeof(buffer2), pipe) != nullptr) {
            buffer << buffer2;
        }

        // Close the pipe
        pclose(pipe);

        // Return the result
        return buffer.str();
    } catch (...) {
        // Handle any exceptions and return an empty string
        return "";
    }
}
```
```