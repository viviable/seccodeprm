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
        std::cerr << "Failed to open archive" << std::endl;
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *path = archive_entry_pathname(entry);
        mode_t mode = archive_entry_mode(entry);
        uid_t uid = archive_entry_uid(entry);
        gid_t gid = archive_entry_gid(entry);
        time_t mtime = archive_entry_mtime(entry);
        off_t size = archive_entry_size(entry);

        // Create directory structure
        std::string full_path = dest_path + "/" + path;
        std::string parent_path = full_path.substr(0, full_path.find_last_of('/'));
        if (!parent_path.empty()) {
            system(("mkdir -p \"" + parent_path + "\"").c_str());
        }

        // Handle directories
        if (S_ISDIR(mode)) {
            system(("mkdir -p \"" + full_path + "\"").c_str());
            continue;
        }

        // Handle files
        FILE *f = fopen(full_path.c_str(), "wb");
        if (!f) {
            std::cerr << "Failed to create file: " << full_path << std::endl;
            return false;
        }

        const void *buff;
        size_t size_read;
        off_t offset_read;
        while ((r = archive_read_data_block(a, &buff, &size_read, &offset_read)) == ARCHIVE_OK) {
            fwrite(buff, 1, size_read, f);
        }
        fclose(f);

        // Set permissions
        chmod(full_path.c_str(), mode);
        chown(full_path.c_str(), uid, gid);
        utime(full_path.c_str(), &mtime);
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```