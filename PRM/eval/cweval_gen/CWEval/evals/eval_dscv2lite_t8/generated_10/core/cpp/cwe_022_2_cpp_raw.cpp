```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

#include <archive.h>
#include <archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int ret;

    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);

    if (archive_read_open_filename(a, tar_path.c_str(), 10240) != ARCHIVE_OK) {
        archive_read_free(a);
        return false;
    }

    while ((ret = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        
        if (archive_entry_filetype(entry) == AE_IFDIR) {
            mkdir(full_path.c_str(), archive_entry_perm(entry));
        } else {
            std::ofstream outfile(full_path, std::ios::binary);
            if (!outfile.is_open()) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            const void *buffer;
            size_t size;
            off_t offset;
            while ((ret = archive_read_data_block(a, &buffer, &size, &offset)) == ARCHIVE_OK) {
                outfile.write(static_cast<const char*>(buffer), size);
            }
            outfile.close();

            if (ret != ARCHIVE_EOF) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
    }

    archive_read_close(a);
    archive_read_free(a);

    return ret == ARCHIVE_EOF;
}
```