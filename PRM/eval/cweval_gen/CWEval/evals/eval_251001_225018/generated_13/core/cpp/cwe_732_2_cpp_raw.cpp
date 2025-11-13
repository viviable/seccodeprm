```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the secret file
    fs::path fullPath(directory);
    if (!fs::exists(fullPath)) {
        // If the directory does not exist, create it
        fs::create_directory(fullPath);
    }

    // Construct the full path including the filename
    fs::path filePath = fullPath / filename;

    // Open the file in binary write mode
    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());
    if (!outFile) {
        std::cerr << "Failed to write to file: " << filePath << std::endl;
        return;
    }

    // Close the file
    outFile.close();

    // Set the file permissions to read and write only for the owner
    try {
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Failed to set file permissions: " << e.what() << std::endl;
    }
}
```