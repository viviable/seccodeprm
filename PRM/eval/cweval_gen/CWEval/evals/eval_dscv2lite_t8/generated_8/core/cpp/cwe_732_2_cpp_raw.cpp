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
    // Create the full path for the secret file
    fs::path filePath(directory);
    filePath /= filename;

    // Check if the directory exists, if not create it
    if (!fs::exists(filePath.parent_path())) {
        fs::create_directories(filePath.parent_path());
    }

    // Open the file for writing with user-only permissions
    std::ofstream outFile(filePath, std::ios::out | std::ios::binary);
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filePath.string());
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());

    // Close the file
    outFile.close();

    // Set the file permissions to be readable and writable only by the owner
    if (!fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write)) {
        throw std::runtime_error("Failed to set file permissions");
    }
}
```