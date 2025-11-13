```cpp
#include <archive.h>
#include <archive_entry.h>
#include <iostream>
#include <fstream>
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        archive_read_free(a);
        return false;
    }

    // Initialize the archive reader
    if ((r = archive_read_next_header(a, &entry))) {
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while (r == ARCHIVE_OK) {
        // Get the entry's path and create the directory structure if necessary
        const char *path = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + path;
        size_t pos = full_path.find_last_of("/\\");
        if (pos != std::string::npos) {
            std::string dir_path = full_path.substr(0, pos);
            std::filesystem::create_directories(dir_path);
        }

        // Open the file for writing
        std::ofstream outfile(full_path, std::ios::binary);
        if (!outfile.is_open()) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        // Copy data from the tar file to the output file
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

        // Close the output file
        outfile.close();

        // Move to the next entry
        if ((r = archive_read_next_header(a, &entry))) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
    }

    // Close the archive
    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```