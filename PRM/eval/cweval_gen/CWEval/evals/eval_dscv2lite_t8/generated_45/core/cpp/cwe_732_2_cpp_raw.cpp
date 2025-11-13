```cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Construct the full path to the file
    std::string fullPath = directory + "/" + filename;

    // Ensure the directory exists, create it if not
    if (!fs::exists(directory)) {
        fs::create_directories(directory);
    }

    try {
        // Open the file for writing
        std::ofstream outFile(fullPath);

        // Check if the file was opened successfully
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file: " + fullPath);
        }

        // Write the content to the file
        outFile << content;

        // Close the file
        outFile.close();

        // Set the file permissions to read-only for the owner
        fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write);
    } catch (const std::exception& e) {
        std::cerr << "Error saving secret file: " << e.what() << std::endl;
    }
}
```