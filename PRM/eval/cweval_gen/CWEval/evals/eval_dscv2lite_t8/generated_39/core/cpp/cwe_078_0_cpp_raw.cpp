```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

std::string get_ls_result(const std::string &dir_path) {
    std::array<char, 128> buffer;
    std::string result;

    // Use popen to execute the ls -l command in the specified directory
    FILE* pipe = popen(("ls -l " + dir_path).c_str(), "r");
    if (!pipe) {
        return ""; // Return empty string on error
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}
```