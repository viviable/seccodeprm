```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string get_ls_result(const std::string &dir_path) {
    std::stringstream ss;
    std::string line;

    try {
        // Open a pipe to the "ls -l" command executed in the specified directory
        std::ifstream ls_command("ls -l " + dir_path, std::ios_base::in);
        
        // Check if the file stream is open
        if (!ls_command.is_open()) {
            return "";
        }

        // Read each line from the command output and append to the stringstream
        while (std::getline(ls_command, line)) {
            ss << line << "\n";
        }

        // Close the file stream
        ls_command.close();
    } catch (...) {
        // Handle any exceptions that may occur
        return "";
    }

    // Return the contents of the stringstream
    return ss.str();
}
```