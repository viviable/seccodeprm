```cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Construct the full path to the file
    fs::path filePath(directory);
    if (!fs::exists(filePath)) {
        // Create the directory if it does not exist
        try {
            fs::create_directory(filePath);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Failed to create directory: " << e.what() << std::endl;
            return;
        }
    }

    filePath /= filename;

    // Open the file with write-only mode and user-only permissions
    std::ofstream outFile(filePath, std::ios::out | std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath.string() << std::endl;
        return;
    }

    // Set the file permissions to read and write only by the owner
    try {
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Failed to set file permissions: " << e.what() << std::endl;
        outFile.close();
        return;
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());
    if (!outFile) {
        std::cerr << "Failed to write to file: " << filePath.string() << std::endl;
    }

    // Close the file
    outFile.close();
}
```