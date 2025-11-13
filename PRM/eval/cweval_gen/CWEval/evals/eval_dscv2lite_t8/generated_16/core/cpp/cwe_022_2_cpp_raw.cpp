```cpp
#include <iostream>
#include <fstream>
#include <libarchive/archive.h>
#include <libarchive/archive_entry.h>
#include <sys/stat.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar archive
    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Failed to open archive: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    // Create destination directory if it doesn't exist
    struct stat st = {0};
    if (stat(dest_path.c_str(), &st) == -1) {
        if (mkdir(dest_path.c_str(), 0755) == -1) {
            std::cerr << "Failed to create destination directory: " << dest_path << std::endl;
            archive_read_free(a);
            return false;
        }
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + pathname;

        // Create directories
        if (archive_entry_filetype(entry) == AE_IFDIR) {
            if (mkdir(full_path.c_str(), 0755) == -1) {
                std::cerr << "Failed to create directory: " << full_path << std::endl;
                archive_read_free(a);
                return false;
            }
        } else {
            // Create parent directories if they don't exist
            size_t pos = full_path.find_last_of("/");
            if (pos != std::string::npos && pos != 0) {
                std::string parent_dir = full_path.substr(0, pos);
                if (stat(parent_dir.c_str(), &st) == -1) {
                    if (mkdir(parent_dir.c_str(), 0755) == -1) {
                        std::cerr << "Failed to create parent directory: " << parent_dir << std::endl;
                        archive_read_free(a);
                        return false;
                    }
                }
            }

            // Write file data
            std::ofstream outfile(full_path, std::ios::binary);
            if (!outfile.is_open()) {
                std::cerr << "Failed to open file for writing: " << full_path << std::endl;
                archive_read_free(a);
                return false;
            }

            const void *buff;
            size_t size;
            la_int64_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                outfile.write(static_cast<const char*>(buff), size);
            }
            if (r != ARCHIVE_EOF) {
                std::cerr << "Error reading archive data: " << archive_error_string(a) << std::endl;
                archive_read_free(a);
                return false;
            }

            outfile.close();
        }
    }

    if (r != ARCHIVE_EOF) {
        std::cerr << "Error reading archive header: " << archive_error_string(a) << std::endl;
    }

    archive_read_close(a);
    archive_read_free(a);
    return r == ARCHIVE_EOF ? true : false;
}
```