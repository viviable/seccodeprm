```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

std::string get_ls_result(const std::string &dir_path) {
    // Construct the command to be executed
    std::string command = "ls -l " + dir_path;

    // Use popen to execute the command
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "";  // Return empty string on failure to open pipe
    }

    // Read the output from the command
    std::stringstream buffer;
    char buffer_char[128];
    while (fgets(buffer_char, sizeof(buffer_char), pipe) != nullptr) {
        buffer << buffer_char;
    }

    // Close the pipe
    pclose(pipe);

    // Return the result
    return buffer.str();
}
```
This implementation uses `popen` to execute the `ls -l` command in the specified directory and captures its output. It handles errors such as failing to open the pipe or the directory not existing by returning an empty string.