```cpp
#include <iostream>
#include <string>
#include <archive.h>
#include <archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a = archive_read_new();
    struct archive_entry *entry;
    int r;

    // Set up the archive reader
    if (archive_read_support_filter_all(a) != ARCHIVE_OK ||
        archive_read_support_format_tar(a) != ARCHIVE_OK) {
        archive_read_free(a);
        return false;
    }

    // Open the tar file
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        fprintf(stderr, "Failed to open archive\n");
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        mode_t mode = archive_entry_mode(entry);
        uid_t uid = archive_entry_uid(entry);
        gid_t gid = archive_entry_gid(entry);
        time_t mtime = archive_entry_mtime(entry);
        off_t size = archive_entry_size(entry);

        // Create directories and files
        if (S_ISDIR(mode)) {
            mkdir(pathname, mode);
        } else {
            FILE *f = fopen(pathname, "wb");
            if (!f) {
                fprintf(stderr, "Failed to create file %s\n", pathname);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            while ((r = archive_read_data(a, f, size)) == ARCHIVE_OK) {
                // Continue reading until the end of the file
            }
            fclose(f);
            if (r != ARCHIVE_EOF) {
                fprintf(stderr, "Error reading file %s\n", pathname);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }

        // Set permissions, ownership, and timestamps
        chmod(pathname, mode);
        chown(pathname, uid, gid);
        utime(pathname, new struct timeval {mtime, mtime});
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```