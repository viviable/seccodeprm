```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Construct the full path to the file
    fs::path filePath(directory);
    filePath /= filename;

    try {
        // Create the directory if it does not exist
        if (!fs::exists(filePath.parent_path())) {
            fs::create_directories(filePath.parent_path());
        }

        // Open the file for writing
        std::ofstream outFile(filePath, std::ios::binary | std::ios::trunc);

        // Check if the file was opened successfully
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

        // Close the file
        outFile.close();

        // Set the file permissions to be readable and writable only by the owner
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);

        std::cout << "File saved successfully: " << filePath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving file: " << e.what() << std::endl;
    }
}
```