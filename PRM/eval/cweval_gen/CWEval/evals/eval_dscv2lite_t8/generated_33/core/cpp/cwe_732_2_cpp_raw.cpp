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

    // Check if the directory exists, if not create it
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
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return;
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());
    if (!outFile) {
        std::cerr << "Failed to write content to file: " << filePath << std::endl;
        outFile.close();
        return;
    }

    // Set the file permissions to be readable and writable only by the owner
    try {
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error setting file permissions: " << e.what() << std::endl;
    }

    // Close the file
    outFile.close();
}
```