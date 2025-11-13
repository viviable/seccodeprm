```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

std::string get_ls_result(const std::string &dir_path) {
    std::string result;
    std::array<char, 128> buffer;
    FILE* pipe = popen(("ls -l " + dir_path).c_str(), "r");
    if (!pipe) return result;

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}
```