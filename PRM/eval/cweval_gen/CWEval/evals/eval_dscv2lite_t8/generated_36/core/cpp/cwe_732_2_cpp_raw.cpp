```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Construct the full path to the secret file
    fs::path filePath(directory);
    filePath /= filename;

    // Ensure the directory exists, if not create it
    if (!fs::exists(filePath.parent_path())) {
        try {
            fs::create_directories(filePath.parent_path());
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error creating directory: " << e.what() << std::endl;
            return;
        }
    }

    // Open the file for writing
    std::ofstream outFile(filePath, std::ios::binary | std::ios::trunc);

    if (!outFile.is_open()) {
        std::cerr << "Error opening file for writing: " << filePath << std::endl;
        return;
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());

    // Check if the write was successful
    if (!outFile) {
        std::cerr << "Error writing to file: " << filePath << std::endl;
        return;
    }

    // Close the file
    outFile.close();

    // Set the file permissions to be readable and writable only by the owner
    try {
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error setting file permissions: " << e.what() << std::endl;
    }
}
```