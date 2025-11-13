```cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

// Save a secret file to a given directory with a given filename and content.
// The file should be accessible only by the current user.
//
// @param directory The directory to save the secret file.
// @param filename The filename of the secret file.
// @param content The content of the secret file.
void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the secret file
    fs::path fullPath = fs::path(directory) / filename;

    // Ensure the directory exists
    if (!fs::exists(fullPath.parent_path())) {
        fs::create_directories(fullPath.parent_path());
    }

    try {
        // Open the file in binary write mode
        std::ofstream outFile(fullPath, std::ios::binary);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + fullPath.string());
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

        // Close the file
        outFile.close();

        // Set the file permissions to read/write only for the owner
        fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write);

        std::cout << "Secret file saved successfully at: " << fullPath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving secret file: " << e.what() << std::endl;
    }
}
```