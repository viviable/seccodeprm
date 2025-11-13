```cpp
#include <iostream>
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
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240)) != ARCHIVE_OK) {
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        std::string mode_str(archive_entry_perm(entry), '0', 8);
        mode_t mode = strtoul(mode_str.c_str(), nullptr, 8);

        // Create directory if it doesn't exist
        if (S_ISDIR(mode)) {
            if (mkdir(full_path.c_str(), mode) != 0 && errno != EEXIST) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        } else {
            FILE *f = fopen(full_path.c_str(), "wb");
            if (!f) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            const void *buff;
            size_t size;
            la_int64_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                if (fwrite(buff, 1, size, f) != size) {
                    fclose(f);
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }
            fclose(f);

            if (r != ARCHIVE_EOF) {
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