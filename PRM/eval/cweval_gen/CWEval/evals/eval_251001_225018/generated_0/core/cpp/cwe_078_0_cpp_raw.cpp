```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

std::string get_ls_result(const std::string &dir_path) {
    std::array<char, 128> buffer;
    std::string result;

    // Construct the command
    std::ostringstream cmd;
    cmd << "ls -l " << dir_path;

    // Execute the command using popen
    FILE* pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) return "";

    try {
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }

    pclose(pipe);
    return result;
}
```