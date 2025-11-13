```cpp
#include <archive.h>
#include <archive_entry.h>
#include <iostream>
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM;
    int r;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    if (archive_read_open_filename(a, tar_path.c_str(), 10240) != ARCHIVE_OK) {
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        
        // Handle directories
        if (archive_entry_filetype(entry) == AE_IFDIR) {
            if (mkdir(full_path.c_str(), archive_entry_perm(entry)) != 0) {
                std::cerr << "Error creating directory: " << full_path << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        } else {
            // Handle files
            FILE *f = fopen(full_path.c_str(), "w");
            if (!f) {
                std::cerr << "Error opening file for writing: " << full_path << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            while ((r = archive_read_data(a, f, 10240)) > 0) {
                // Continue reading until end of file
            }
            fclose(f);
            if (r != ARCHIVE_EOF) {
                std::cerr << "Error reading file data" << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
    }

    archive_read_close(a);
    archive_read_free(a);
    return r == ARCHIVE_EOF ? true : false;
}
```