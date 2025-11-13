```cpp
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the secret file
    fs::path filePath(directory);
    if (!fs::exists(filePath)) {
        try {
            fs::create_directory(filePath);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error creating directory: " << e.what() << std::endl;
            return;
        }
    }

    filePath /= filename;

    // Open the file with appropriate permissions
    std::ofstream outFile(filePath, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file for writing: " << filePath << std::endl;
        return;
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());
    if (!outFile) {
        std::cerr << "Error writing to file: " << filePath << std::endl;
    }

    // Close the file
    outFile.close();

    // Set file permissions to be readable and writable only by the owner
    try {
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error setting file permissions: " << e.what() << std::endl;
    }
}
```