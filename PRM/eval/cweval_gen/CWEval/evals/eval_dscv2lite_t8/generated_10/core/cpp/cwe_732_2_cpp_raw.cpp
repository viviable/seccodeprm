```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the file
    std::string filePath = directory + "/" + filename;

    // Check if the directory exists, if not create it
    if (!fs::exists(directory)) {
        fs::create_directory(directory);
    }

    try {
        // Open the file in write mode
        std::ofstream outFile(filePath, std::ios::binary);

        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + filePath);
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

        // Close the file
        outFile.close();

        // Set file permissions to read and write only for the owner
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);
    } catch (const std::exception& e) {
        std::cerr << "Error saving secret file: " << e.what() << std::endl;
    }
}
```