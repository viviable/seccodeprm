```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <libarchive/archive.h>
#include <libarchive/archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a = archive_read_new();
    struct archive_entry *entry;
    int r;

    if (a == nullptr) {
        return false;
    }

    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);

    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        archive_read_free(a);
        return false;
    }

    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string path = dest_path + "/" + archive_entry_pathname(entry);
        mode_t mode = archive_entry_mode(entry);
        uid_t uid = archive_entry_uid(entry);
        gid_t gid = archive_entry_gid(entry);
        time_t mtime = archive_entry_mtime(entry);
        off_t size = archive_entry_size(entry);

        if (S_ISDIR(mode)) {
            mkdir(path.c_str(), mode);
        } else {
            std::ofstream ofs(path, std::ios::binary);
            if (!ofs.is_open()) {
                archive_read_free(a);
                return false;
            }
            while ((r = archive_read_data(a, ofs.data(), ofs.size())) > 0) {
                ofs.write(ofs.data(), r);
            }
            ofs.close();
        }
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```