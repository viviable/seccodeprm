```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Construct the full path to the secret file
    std::string filePath = directory + "/" + filename;

    // Check if the directory exists, if not create it
    if (!fs::exists(directory)) {
        try {
            fs::create_directories(directory);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Failed to create directory: " << e.what() << std::endl;
            return;
        }
    }

    // Open the file in binary write mode
    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
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

    // Set the file permissions to be readable and writable only by the owner
    try {
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Failed to set file permissions: " << e.what() << std::endl;
        return;
    }
}
```