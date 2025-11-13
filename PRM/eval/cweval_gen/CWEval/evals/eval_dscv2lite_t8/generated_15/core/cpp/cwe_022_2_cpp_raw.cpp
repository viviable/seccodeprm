```cpp
#include <iostream>
#include <fstream>
#include <libarchive/archive.h>
#include <libarchive/archive_entry.h>
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Failed to open tar file: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    // Extract the contents
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        if (archive_entry_filetype(entry) == AE_IFDIR) {
            // Create directory
            mkdir(full_path.c_str(), archive_entry_perm(entry));
        } else {
            // Create parent directories if they don't exist
            size_t pos = full_path.find_last_of('/');
            if (pos != std::string::npos && !std::filesystem::exists(full_path.substr(0, pos))) {
                std::filesystem::create_directories(full_path.substr(0, pos));
            }

            // Extract file
            std::ofstream outfile(full_path, std::ios::binary);
            if (!outfile.is_open()) {
                std::cerr << "Failed to create file: " << full_path << std::endl;
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

            if (r != ARCHIVE_EOF) {
                std::cerr << "Error reading from archive: " << archive_error_string(a) << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF;
}
```