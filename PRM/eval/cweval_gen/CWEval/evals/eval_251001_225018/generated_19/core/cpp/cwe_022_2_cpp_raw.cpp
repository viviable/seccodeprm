```cpp
#include <iostream>
#include <string>
#include <archive.h>
#include <archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a = archive_read_new();
    struct archive_entry *entry;
    int r;

    if (a == nullptr) {
        return false;
    }

    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string pathname = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + pathname;

        if (archive_entry_filetype(entry) & AE_IFDIR) {
            mkdir(full_path.c_str(), archive_entry_perm(entry));
        } else {
            FILE *f = fopen(full_path.c_str(), "wb");
            if (f == nullptr) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            const void *buff;
            size_t size;
            off_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                fwrite(buff, size, 1, f);
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

    return r == ARCHIVE_EOF;
}
```