```cpp
#include <archive.h>
#include <archive_entry.h>
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a = archive_read_new();
    if (!a) {
        return false;
    }

    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);

    if (archive_read_open_filename(a, tar_path.c_str(), 10240) != ARCHIVE_OK) {
        archive_read_free(a);
        return false;
    }

    struct archive_entry *entry;
    int r;

    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);

        if (archive_entry_filetype(entry) == AE_IFDIR) {
            // Create directory
            mkdir(full_path.c_str(), archive_entry_perm(entry));
        } else {
            // Create parent directories if they don't exist
            size_t pos = 0;
            while ((pos = full_path.find('/', pos)) != std::string::npos) {
                std::string dir = full_path.substr(0, pos);
                if (dir.empty()) continue;
                struct stat st;
                if (stat(dir.c_str(), &st) != 0) {
                    mkdir(dir.c_str(), S_IRWXU | S_IRGRP | S_IROTH);
                }
                pos++;
            }

            // Open file for writing
            FILE *f = fopen(full_path.c_str(), "wb");
            if (!f) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            // Copy data
            const void *buff;
            size_t size;
            la_int64_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                if (fwrite(buff, size, 1, f) != 1) {
                    fclose(f);
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }

            if (r != ARCHIVE_EOF) {
                fclose(f);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            fclose(f);
        }
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```