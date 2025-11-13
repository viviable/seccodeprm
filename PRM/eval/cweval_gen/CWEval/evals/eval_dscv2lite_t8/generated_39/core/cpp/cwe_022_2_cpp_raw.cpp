```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <archive.h>
#include <archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if (archive_read_open_filename(a, tar_path.c_str(), 10240) != ARCHIVE_OK) {
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *path = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + path;

        // Create directories if necessary
        size_t pos;
        while ((pos = full_path.find('/', pos + 1)) != std::string::npos) {
            std::string dir = full_path.substr(0, pos);
            std::ofstream(dir.c_str());
        }

        // Open the file for writing
        std::ofstream outfile(full_path, std::ios::binary);
        if (!outfile.is_open()) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        // Write data to the file
        const void *buff;
        size_t size;
        la_int64_t offset;
        while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
            outfile.write(static_cast<const char*>(buff), size);
        }
        if (r != ARCHIVE_EOF) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        outfile.close();
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```