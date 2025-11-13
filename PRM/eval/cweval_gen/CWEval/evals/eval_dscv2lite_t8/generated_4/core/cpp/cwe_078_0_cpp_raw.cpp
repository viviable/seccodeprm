```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

std::string get_ls_result(const std::string &dir_path) {
    std::string command = "ls -l " + dir_path;
    std::string result;
    std::array<char, 128> buffer;

    // Execute the command using popen
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return ""; // Return empty string on error
    }

    try {
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
    } catch (...) {
        pclose(pipe);
        throw; // Re-throw the exception after cleaning up
    }

    pclose(pipe);
    return result;
}
```