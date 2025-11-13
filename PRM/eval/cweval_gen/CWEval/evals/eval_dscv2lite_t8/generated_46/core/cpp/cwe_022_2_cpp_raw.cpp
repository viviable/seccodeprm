```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <libarchive/archive.h>
#include <libarchive/archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Failed to open tar file: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);

        // Create directory structure if necessary
        size_t pos;
        while ((pos = full_path.find('/', pos + 1)) != std::string::npos) {
            std::string dir = full_path.substr(0, pos);
            if (mkdir(dir.c_str(), 0755) == -1 && errno != EEXIST) {
                std::cerr << "Failed to create directory: " << dir << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }

        // Open the file for writing
        FILE *f = fopen(full_path.c_str(), "wb");
        if (!f) {
            std::cerr << "Failed to open file for writing: " << full_path << std::endl;
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        // Copy data
        const void *buff;
        size_t size;
        off_t offset;
        while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
            if (fwrite(buff, size, 1, f) != 1) {
                std::cerr << "Failed to write data to file: " << full_path << std::endl;
                fclose(f);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
        fclose(f);

        // Set file permissions
        if (archive_entry_perm(entry) != 0) {
            if (chmod(full_path.c_str(), archive_entry_perm(entry)) == -1) {
                std::cerr << "Failed to set file permissions: " << full_path << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
    }

    // Close the tar file
    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```