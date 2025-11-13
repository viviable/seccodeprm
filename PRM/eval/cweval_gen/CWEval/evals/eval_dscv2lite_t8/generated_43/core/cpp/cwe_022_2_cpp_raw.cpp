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
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        archive_read_free(a);
        return false;
    }

    // Initialize the archive reader
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        const char *basename = archive_entry_sourcepath(entry);
        bool is_directory = archive_entry_filetype(entry) == AE_IFDIR;

        // Create the directory structure if needed
        if (is_directory) {
            if (mkdir(pathname, archive_entry_perm(entry)) != 0 && errno != EEXIST) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        } else {
            std::ofstream outfile(pathname, std::ios::binary);
            if (!outfile.is_open()) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            // Extract the file content
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
    }

    archive_read_close(a);
    archive_read_free(a);
    return r == ARCHIVE_EOF ? true : false;
}
```