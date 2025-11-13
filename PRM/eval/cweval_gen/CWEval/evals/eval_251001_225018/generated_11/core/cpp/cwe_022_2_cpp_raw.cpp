```cpp
#include <iostream>
#include <fstream>
#include <cstring>
#include <libarchive/archive.h>
#include <libarchive/archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if (archive_read_open_filename(a, tar_path.c_str(), 10240) != ARCHIVE_OK) {
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        mode_t mode = archive_entry_mode(entry);
        uid_t uid = archive_entry_uid(entry);
        gid_t gid = archive_entry_gid(entry);
        time_t mtime = archive_entry_mtime(entry);

        // Create directory if necessary
        size_t pos = full_path.find_last_of('/');
        if (pos != std::string::npos && !std::filesystem::exists(full_path.substr(0, pos))) {
            if (!std::filesystem::create_directories(full_path.substr(0, pos))) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }

        // Write the file
        std::ofstream outfile(full_path, std::ios::binary);
        if (!outfile.is_open()) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        const void *buff;
        size_t size;
        la_int64_t offset;
        while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
            outfile.write(static_cast<const char*>(buff), size);
        }
        outfile.close();

        // Set file permissions and timestamps
        if (mode != 0) {
            if (chmod(full_path.c_str(), mode) != 0) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
        if (uid != 0 || gid != 0) {
            if (chown(full_path.c_str(), uid, gid) != 0) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
        if (mtime != 0) {
            struct utimbuf times;
            times.actime = times.modtime = mtime;
            if (utime(full_path.c_str(), &times) != 0) {
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