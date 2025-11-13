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
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        
        // Handle directories
        if (S_ISDIR(archive_entry_filetype(entry))) {
            if (mkdir(full_path.c_str(), archive_entry_perm(entry)) != 0) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        } else {
            // Handle files
            std::ofstream outfile(full_path, std::ios::binary);
            if (!outfile) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            const void *buff;
            size_t size;
            off_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                outfile.write(static_cast<const char*>(buff), size);
            }
            if (r != ARCHIVE_EOF || !outfile.good()) {
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