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
    // Create the full path for the file
    fs::path filePath(directory);
    filePath /= filename;

    // Ensure the directory exists
    if (!fs::exists(filePath.parent_path())) {
        fs::create_directories(filePath.parent_path());
    }

    // Open the file with appropriate permissions
    std::ofstream outFile(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());

    // Set the file permissions to be readable and writable only by the owner
    if (!fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write)) {
        std::cerr << "Failed to set file permissions: " << filePath << std::endl;
    }

    // Close the file
    outFile.close();
}
```