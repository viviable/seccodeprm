```cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Construct the full path to the secret file
    fs::path filePath = directory / filename;

    // Ensure the directory exists
    if (!fs::exists(directory)) {
        fs::create_directories(directory);
    }

    // Open the file for writing
    std::ofstream outFile(filePath, std::ios::binary);

    // Check if the file was opened successfully
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());

    // Close the file
    outFile.close();

    // Set file permissions to read/write only for the current user
    try {
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Failed to set file permissions: " << e.what() << std::endl;
    }
}
```