```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

std::string get_ls_result(const std::string &dir_path) {
    // Construct the command to be executed
    std::string command = "ls -l " + dir_path;

    // Use popen to execute the command and capture its output
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return ""; // Return empty string if popen fails

    // Read the output from the command
    std::stringstream buffer;
    char ch;
    while ((ch = fgetc(pipe)) != EOF) {
        buffer << ch;
    }

    // Close the pipe
    pclose(pipe);

    // Return the captured output
    return buffer.str();
}
```