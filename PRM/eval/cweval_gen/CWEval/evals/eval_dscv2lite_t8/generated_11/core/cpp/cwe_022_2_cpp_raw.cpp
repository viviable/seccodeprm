```cpp
#include <iostream>
#include <string>
#include <archive.h>
#include <archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar archive
    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Failed to open archive: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    // Extract the archive
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string pathname = dest_path + "/" + archive_entry_pathname(entry);
        mode_t mode = archive_entry_mode(entry);
        uid_t uid = archive_entry_uid(entry);
        gid_t gid = archive_entry_gid(entry);
        time_t mtime = archive_entry_mtime(entry);

        // Create directories and files
        if (S_ISDIR(mode)) {
            if (mkdir(pathname.c_str(), mode) != 0) {
                std::cerr << "Failed to create directory: " << pathname << " Error: " << strerror(errno) << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        } else {
            FILE *f = fopen(pathname.c_str(), "w");
            if (!f) {
                std::cerr << "Failed to create file: " << pathname << " Error: " << strerror(errno) << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            while ((r = archive_read_data(a, f, 8192)) > 0) {
                // Continue reading data
            }
            fclose(f);
            if (r < ARCHIVE_OK) {
                std::cerr << "Failed to read data from archive: " << archive_error_string(a) << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }

        // Set permissions, ownership, and timestamps
        chmod(pathname.c_str(), mode);
        chown(pathname.c_str(), uid, gid);
        utime(pathname.c_str(), &mtime);
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```