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
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        mode_t mode = archive_entry_perm(entry);

        // Ensure the directory structure exists
        size_t pos = 0;
        while ((pos = full_path.find('/', pos)) != std::string::npos) {
            std::string dir_path = full_path.substr(0, pos);
            if (mkdir(dir_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH) == -1 && errno != EEXIST) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            ++pos;
        }

        // Write the file
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

        // Set permissions
        chmod(full_path.c_str(), mode);
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```