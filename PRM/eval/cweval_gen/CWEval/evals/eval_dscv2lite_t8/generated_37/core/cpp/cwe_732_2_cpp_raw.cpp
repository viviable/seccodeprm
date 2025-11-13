```cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// Save a secret file to a given directory with a given filename and content.
// The file should be accessible only by the current user.
//
// @param directory The directory to save the secret file.
// @param filename The filename of the secret file.
// @param content The content of the secret file.
void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Construct the full path to the file
    fs::path filePath(directory);
    if (!fs::exists(filePath)) {
        try {
            fs::create_directories(filePath);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Failed to create directory: " << e.what() << std::endl;
            return;
        }
    }

    filePath /= filename;

    // Open the file with write permissions for the owner only
    std::ofstream outFile(filePath, std::ios::out | std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return;
    }

    // Set the file permissions to read and write for the owner only
    try {
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Failed to set file permissions: " << e.what() << std::endl;
        outFile.close();
        return;
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());
    if (!outFile) {
        std::cerr << "Failed to write content to file: " << filePath << std::endl;
    }

    // Close the file
    outFile.close();
}
```