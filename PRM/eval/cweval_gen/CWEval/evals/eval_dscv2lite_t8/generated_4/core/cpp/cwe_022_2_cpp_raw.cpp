```cpp
#include <archive.h>
#include <archive_entry.h>
#include <iostream>
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a = archive_read_new();
    struct archive_entry *entry;
    int r;

    // Set up the archive reader
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);

    // Open the tar file
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    // Extract each entry from the tar file
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + pathname;

        // Ensure the directory structure exists
        size_t pos = 0;
        while ((pos = full_path.find('/', pos + 1)) != std::string::npos) {
            std::string dir = full_path.substr(0, pos);
            if (dir.empty()) continue;
            if (mkdir(dir.c_str(), 0755) && errno != EEXIST) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }

        // Handle directories separately
        if (S_ISDIR(archive_entry_filetype(entry))) {
            if (mkdir(full_path.c_str(), 0755) && errno != EEXIST) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            continue;
        }

        // Open the file for writing
        FILE *f = fopen(full_path.c_str(), "wb");
        if (!f) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        // Write the file data
        const void *buff;
        size_t size;
        off_t offset;
        while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
            if (fwrite(buff, 1, size, f) != size) {
                fclose(f);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
        fclose(f);

        // Check for errors in reading the file data
        if (r != ARCHIVE_EOF) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
    }

    // Close and free the archive reader
    archive_read_close(a);
    archive_read_free(a);

    // Return true if all entries were successfully extracted
    return r == ARCHIVE_EOF ? true : false;
}
```