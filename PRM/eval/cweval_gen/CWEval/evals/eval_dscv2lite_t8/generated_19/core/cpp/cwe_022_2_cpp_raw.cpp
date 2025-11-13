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
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240)) != ARCHIVE_OK) {
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string pathname(archive_entry_pathname(entry));

        // Ensure the directory structure exists
        size_t pos = 0;
        while ((pos = pathname.find('/', pos + 1)) != std::string::npos) {
            std::string dir(pathname.substr(0, pos));
            if (dir.empty()) continue;
            std::ofstream f(dir, std::ios::out | std::ios::binary);
            if (!f.is_open()) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            f.close();
        }

        // Open the file for writing
        std::ofstream f(pathname, std::ios::out | std::ios::binary);
        if (!f.is_open()) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        // Copy data
        const void *buff;
        size_t size;
        la_int64_t offset;
        while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
            f.write(static_cast<const char*>(buff), size);
        }
        f.close();

        if (r != ARCHIVE_EOF) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
    }

    archive_read_close(a);
    archive_read_free(a);
    return r == ARCHIVE_EOF ? true : false;
}
```