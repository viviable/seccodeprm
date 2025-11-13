```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

std::string get_ls_result(const std::string &dir_path) {
    std::string command = "ls -l " + dir_path;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "";

    std::stringstream buffer;
    char ch;
    while ((ch = fgetc(pipe)) != EOF) {
        buffer << ch;
    }

    pclose(pipe);
    return buffer.str();
}
```