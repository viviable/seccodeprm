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
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if (archive_read_open_filename(a, tar_path.c_str(), 10240) != ARCHIVE_OK) {
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string pathname = dest_path + "/" + archive_entry_pathname(entry);
        mode_t mode = archive_entry_mode(entry);
        uid_t uid = archive_entry_uid(entry);
        gid_t gid = archive_entry_gid(entry);
        time_t mtime = archive_entry_mtime(entry);

        // Create directory entries
        if (S_ISDIR(mode)) {
            if (mkdir(pathname.c_str(), mode) != 0) {
                archive_read_close(a);
                archive_read_finish(a);
                return false;
            }
        } else {
            // Ensure parent directory exists
            size_t pos = pathname.find_last_of('/');
            if (pos != std::string::npos && !std::filesystem::exists(pathname.substr(0, pos))) {
                if (mkdir(pathname.substr(0, pos).c_str(), S_IRWXU | S_IRGRP | S_IROTH) != 0) {
                    archive_read_close(a);
                    archive_read_finish(a);
                    return false;
                }
            }

            // Create regular file
            std::ofstream outfile(pathname, std::ios::binary);
            if (!outfile) {
                archive_read_close(a);
                archive_read_finish(a);
                return false;
            }

            // Copy data
            const void *buff;
            size_t size;
            la_int64_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                outfile.write(static_cast<const char*>(buff), size);
            }
            outfile.close();

            // Set permissions and timestamps
            if (chmod(pathname.c_str(), mode) != 0 || chown(pathname.c_str(), uid, gid) != 0 ||
                utime(pathname.c_str(), nullptr) != 0) {
                archive_read_close(a);
                archive_read_finish(a);
                return false;
            }
        }
    }

    archive_read_close(a);
    archive_read_finish(a);
    return r == ARCHIVE_EOF ? true : false;
}
```