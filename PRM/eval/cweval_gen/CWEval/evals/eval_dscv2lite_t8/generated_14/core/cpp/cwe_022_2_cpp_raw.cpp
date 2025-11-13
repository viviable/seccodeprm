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
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Failed to open archive" << std::endl;
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        mode_t mode = archive_entry_mode(entry);
        uid_t uid = archive_entry_uid(entry);
        gid_t gid = archive_entry_gid(entry);
        time_t mtime = archive_entry_mtime(entry);
        off_t size = archive_entry_size(entry);

        // Create directories and files
        if (S_ISDIR(mode)) {
            if (mkdir(full_path.c_str(), mode) != 0 && errno != EEXIST) {
                std::cerr << "Failed to create directory: " << full_path << std::endl;
                return false;
            }
        } else {
            FILE *f = fopen(full_path.c_str(), "wb");
            if (!f) {
                std::cerr << "Failed to open file for writing: " << full_path << std::endl;
                return false;
            }
            const void *buff;
            size_t size_read;
            la_int64_t offset_read;
            while ((r = archive_read_data_block(a, &buff, &size_read, &offset_read)) == ARCHIVE_OK) {
                if (fwrite(buff, 1, size_read, f) != size_read) {
                    std::cerr << "Failed to write to file: " << full_path << std::endl;
                    fclose(f);
                    return false;
                }
            }
            fclose(f);
            if (r != ARCHIVE_EOF) {
                std::cerr << "Error reading archive" << std::endl;
                return false;
            }
        }

        // Set file permissions, owner, group, and modification time
        chmod(full_path.c_str(), mode);
        chown(full_path.c_str(), uid, gid);
        utime(full_path.c_str(), nullptr);
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```