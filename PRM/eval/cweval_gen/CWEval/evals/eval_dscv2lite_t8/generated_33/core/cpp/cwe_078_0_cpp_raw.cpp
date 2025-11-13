```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

std::string get_ls_result(const std::string &dir_path) {
    std::stringstream cmd;
#ifdef _WIN32
    cmd << "cmd /c cd \"" << dir_path << "\" && ls -l";
#else
    cmd << "cd " << dir_path << " && ls -l";
#endif

    std::string result = "";
    FILE* pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) return "";

    char buffer[128];
    while (fgets(buffer, 128, pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}
```