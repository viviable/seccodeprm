```cpp
#include <iostream>
#include <string>
#include <libarchive/archive.h>
#include <libarchive/archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar archive
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Failed to open archive: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *path = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + path;

        // Create directories if necessary
        size_t pos = full_path.find_last_of('/');
        if (pos != std::string::npos && pos != full_path.length() - 1) {
            std::string dir_path = full_path.substr(0, pos);
            if (mkdir(dir_path.c_str(), 0755) != 0 && errno != EEXIST) {
                std::cerr << "Failed to create directory: " << dir_path << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }

        // Write the file
        FILE *f = fopen(full_path.c_str(), "wb");
        if (!f) {
            std::cerr << "Failed to open file for writing: " << full_path << std::endl;
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        const void *buff;
        size_t size;
        off_t offset;
        while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
            if (fwrite(buff, 1, size, f) != size) {
                std::cerr << "Failed to write data to file: " << full_path << std::endl;
                fclose(f);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
        fclose(f);

        // Set file permissions
        if (archive_entry_mode(entry) != 0) {
            chmod(full_path.c_str(), archive_entry_mode(entry));
        }
    }

    // Check for errors during extraction
    if (r != ARCHIVE_EOF) {
        std::cerr << "Error during extraction: " << archive_error_string(a) << std::endl;
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    // Clean up
    archive_read_close(a);
    archive_read_free(a);
    return true;
}
```