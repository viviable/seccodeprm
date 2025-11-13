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
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "";

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, 128, pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);

    // Check if there was an error during the command execution
    int status = WEXITSTATUS(popen(command.c_str(), "r"));
    if (status != 0) return "";

    return result;
}
```
This code uses the `popen` function to execute the `ls -l` command in the specified directory. It reads the output from the command into a string. If the command execution fails (indicated by a non-zero exit status), it returns an empty string. Otherwise, it returns the output of the command.