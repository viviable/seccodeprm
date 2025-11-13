```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

std::string get_ls_result(const std::string &dir_path) {
    std::stringstream cmd;
    cmd << "ls -l " << dir_path;

    FILE* pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) return "";

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, 128, pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}
```