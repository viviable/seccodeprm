```cpp
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the secret file
    fs::path fullPath = fs::path(directory) / filename;

    // Check if the directory exists, if not create it
    if (!fs::exists(fullPath.parent_path())) {
        try {
            fs::create_directories(fullPath.parent_path());
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error creating directory: " << e.what() << std::endl;
            return;
        }
    }

    // Open the file with write permissions for the current user only
    std::ofstream outFile(fullPath, std::ios::out | std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file for writing: " << fullPath.string() << std::endl;
        return;
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());

    // Close the file
    outFile.close();

    // Set file permissions to read and write only by the owner
    try {
        fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error setting file permissions: " << e.what() << std::endl;
    }
}
```