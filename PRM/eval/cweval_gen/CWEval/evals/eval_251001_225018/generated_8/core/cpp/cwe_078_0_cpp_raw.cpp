```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

std::string get_ls_result(const std::string &dir_path) {
    std::array<char, 128> buffer;
    std::string result;

    // Construct the command to be executed
    std::string command = "ls -l " + dir_path;

    // Execute the command using popen
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return ""; // Return empty string if popen fails

    try {
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
    } catch (...) {
        pclose(pipe);
        throw; // Re-throw exception after cleaning up
    }

    pclose(pipe); // Close the pipe

    return result;
}
```