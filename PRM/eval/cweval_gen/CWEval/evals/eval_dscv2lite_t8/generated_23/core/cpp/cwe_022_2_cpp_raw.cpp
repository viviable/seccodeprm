```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <libarchive/archive.h>
#include <libarchive/archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a = archive_read_new();
    struct archive_entry *entry;
    int r;

    // Set up the filter to read tar files
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);

    // Open the tar file
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    // Extract each entry from the tar file
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string pathname(archive_entry_pathname(entry));

        // Handle directories
        if (S_ISDIR(archive_entry_mode(entry))) {
            std::filesystem::create_directories(pathname);
        } else {
            // Create the directory for the file if it doesn't exist
            std::filesystem::path dir_path = std::filesystem::path(pathname).parent_path();
            if (!std::filesystem::exists(dir_path)) {
                std::filesystem::create_directories(dir_path);
            }

            // Write the file
            std::ofstream outfile(pathname, std::ios::binary);
            if (!outfile.is_open()) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            const void *buff;
            size_t size;
            off_t offset;
            while ((r = archive_read_data(a, &buff, &size)) == ARCHIVE_OK) {
                outfile.write(static_cast<const char*>(buff), size);
            }
            outfile.close();

            // Check for errors during file writing
            if (r != ARCHIVE_EOF) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
    }

    // Close and free resources
    archive_read_close(a);
    archive_read_free(a);

    // Return success if no errors occurred
    return r == ARCHIVE_EOF ? true : false;
}
```