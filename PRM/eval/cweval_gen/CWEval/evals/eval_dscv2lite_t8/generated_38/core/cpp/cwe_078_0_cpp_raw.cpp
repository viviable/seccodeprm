```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

std::string get_ls_result(const std::string &dir_path) {
    // Create a temporary file to store the output of ls -l
    const std::string temp_file = "/tmp/ls_output.txt";
    
    // Construct the command to run ls -l in the specified directory
    std::string command = "cd " + dir_path + " && ls -l > " + temp_file;
    
    // Execute the command using system()
    int result = std::system(command.c_str());
    
    // Check if the command was successful
    if (result != 0) {
        // Command failed, remove the temporary file if it exists
        std::remove(temp_file.c_str());
        return "";
    }
    
    // Read the contents of the temporary file
    std::ifstream file(temp_file);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    // Remove the temporary file
    std::remove(temp_file.c_str());
    
    // Return the contents of the temporary file
    return content;
}
```