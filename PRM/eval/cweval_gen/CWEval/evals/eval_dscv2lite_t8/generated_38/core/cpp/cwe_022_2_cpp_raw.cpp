```cpp
#include <archive.h>
#include <archive_entry.h>
#include <iostream>
#include <fstream>
#include <filesystem>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Could not open tar file: " << tar_path << std::endl;
        archive_read_free(a);
        return false;
    }

    // Create destination directory if it doesn't exist
    if (!std::filesystem::exists(dest_path)) {
        if (!std::filesystem::create_directory(dest_path)) {
            std::cerr << "Could not create destination directory: " << dest_path << std::endl;
            archive_read_free(a);
            return false;
        }
    }

    // Extract each entry from the tar file
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string pathname = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + pathname;

        // Handle directories
        if (archive_entry_filetype(entry) == AE_IFDIR) {
            if (!std::filesystem::create_directory(full_path)) {
                std::cerr << "Could not create directory: " << full_path << std::endl;
                archive_read_free(a);
                return false;
            }
        } else {
            // Handle regular files
            std::ofstream outfile(full_path, std::ios::binary);
            if (!outfile.is_open()) {
                std::cerr << "Could not open file for writing: " << full_path << std::endl;
                archive_read_free(a);
                return false;
            }

            const void *buff;
            size_t size;
            off_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                outfile.write(static_cast<const char*>(buff), size);
            }
            if (r != ARCHIVE_EOF) {
                std::cerr << "Error reading data block for file: " << full_path << std::endl;
                archive_read_free(a);
                return false;
            }
            outfile.close();
        }
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```