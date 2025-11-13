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
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(("ls -l " + dir_path).c_str(), "r");
    if (!pipe) return "";

    while (fgets(buffer.data(), 128, pipe) != nullptr) {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}
```
This solution uses the `popen` function from the C standard library to execute the `ls -l` command in the specified directory. It reads the output line by line into a buffer and appends it to the result string. If `popen` fails (which can happen due to invalid directory paths or permissions issues), the function returns an empty string.