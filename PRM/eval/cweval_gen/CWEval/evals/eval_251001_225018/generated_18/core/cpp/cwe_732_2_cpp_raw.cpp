```cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the file
    fs::path fullPath = directory / filename;

    // Ensure the directory exists
    if (!fs::exists(directory)) {
        fs::create_directories(directory);
    }

    try {
        // Open the file for writing
        std::ofstream outFile(fullPath, std::ios::binary | std::ios::trunc);

        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + fullPath.string());
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

        // Close the file
        outFile.close();

        // Set the file permissions to be readable and writable only by the owner
        fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving secret file: " << e.what() << std::endl;
    }
}
```