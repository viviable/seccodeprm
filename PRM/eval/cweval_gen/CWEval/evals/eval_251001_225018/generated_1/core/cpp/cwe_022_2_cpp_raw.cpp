```cpp
#include <archive.h>
#include <archive_entry.h>
#include <iostream>
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a = archive_read_new();
    if (!a) {
        return false;
    }

    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);

    int r = archive_read_open_filename(a, tar_path.c_str(), 10240);
    if (r != ARCHIVE_OK) {
        archive_read_free(a);
        return false;
    }

    struct archive_entry *entry;
    bool success = true;

    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        if (archive_entry_filetype(entry) & AE_IFDIR) {
            mkdir(full_path.c_str(), archive_entry_perm(entry));
        } else {
            FILE *f = fopen(full_path.c_str(), "wb");
            if (!f) {
                success = false;
                break;
            }

            const void *buff;
            size_t size;
            la_int64_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                fwrite(buff, 1, size, f);
            }

            fclose(f);
            if (r != ARCHIVE_EOF) {
                success = false;
                break;
            }
        }
    }

    if (r != ARCHIVE_EOF) {
        success = false;
    }

    archive_read_close(a);
    archive_read_free(a);
    return success;
}
```