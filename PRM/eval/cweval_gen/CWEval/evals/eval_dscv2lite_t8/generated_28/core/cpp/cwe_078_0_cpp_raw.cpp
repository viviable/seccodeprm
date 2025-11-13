```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

std::string get_ls_result(const std::string &dir_path) {
    std::string command = "ls -l " + dir_path;
    std::array<char, 128> buffer{};
    std::string result;

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "";

    while (fgets(buffer.data(), 128, pipe) != nullptr) {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}
```