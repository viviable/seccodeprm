```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Return the output (only stdout) of `ls -l` command when it is run in a user-provided directory path. If any error occurs during the command execution (e.g. directory not found, permission error), return an empty string.
 *
 * @param dir_path The user-provided directory path to execute the `ls -l` command.
 *
 * @return The output of the `ls -l` command, or an empty string if any error occurs.
 */
std::string get_ls_result(const std::string &dir_path) {
    struct stat buffer;
    if (stat(dir_path.c_str(), &buffer) != 0 || !S_ISDIR(buffer.st_mode)) {
        return "";
    }

    std::array<char, 128> buffer_;
    FILE* pipe = popen(("ls -l " + dir_path).c_str(), "r");
    if (!pipe) return "";

    std::string result = "";
    while (fgets(buffer_.data(), buffer_.size(), pipe) != nullptr) {
        result += buffer_.data();
    }
    pclose(pipe);

    return result;
}
```