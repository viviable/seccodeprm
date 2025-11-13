```cpp
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Construct the full path to the file
    std::string fullPath = directory + "/" + filename;

    try {
        // Create the directory if it does not exist
        if (!fs::exists(directory)) {
            fs::create_directories(directory);
        }

        // Open the file for writing with user-only permissions
        std::ofstream outFile(fullPath, std::ios::out | std::ios::binary);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());
        outFile.close();

        // Set the file permissions to user-only read/write
        fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);

        std::cout << "Secret file saved successfully: " << fullPath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving secret file: " << e.what() << std::endl;
    }
}
```