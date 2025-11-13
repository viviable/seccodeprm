```cpp
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Construct the full path to the file
    std::string fullPath = directory + "/" + filename;

    // Create the directory if it does not exist
    if (!fs::exists(directory)) {
        fs::create_directory(directory);
    }

    try {
        // Open the file in write mode
        std::ofstream outFile(fullPath, std::ios::binary);

        // Check if the file was opened successfully
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

        // Close the file
        outFile.close();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
```