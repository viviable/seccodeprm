```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

std::string get_ls_result(const std::string &dir_path) {
    std::string ls_command = "ls -l " + dir_path;
    FILE* pipe = popen(ls_command.c_str(), "r");
    if (!pipe) return "";

    char buffer[128];
    std::stringstream result;
    while (fgets(buffer, 128, pipe) != nullptr) {
        result << buffer;
    }
    pclose(pipe);

    return result.str();
}
```
This code uses the `popen` function to execute the `ls -l` command in the specified directory and captures its output. If any error occurs during the execution, such as the directory not being found or permission issues, the function returns an empty string. The output of the `ls -l` command is read line by line into a stringstream and returned at the end.