```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

#include <archive.h>
#include <archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a = archive_read_new();
    struct archive_entry *entry;
    int r;

    if (a == NULL) {
        return false;
    }

    if (archive_read_support_format_all(a) != ARCHIVE_OK ||
        archive_read_support_filter_all(a) != ARCHIVE_OK) {
        archive_read_free(a);
        return false;
    }

    if (archive_read_open_filename(a, tar_path.c_str(), 10240) != ARCHIVE_OK) {
        archive_read_free(a);
        return false;
    }

    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        mode_t mode = archive_entry_perm(entry);
        uid_t uid = archive_entry_uid(entry);
        gid_t gid = archive_entry_gid(entry);
        time_t mtime = archive_entry_mtime(entry);
        dev_t rdev = archive_entry_rdev(entry);
        off_t size = archive_entry_size(entry);
        bool is_dir = S_ISDIR(mode);

        if (is_dir) {
            if (mkdir(full_path.c_str(), mode) != 0) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        } else {
            FILE *f = fopen(full_path.c_str(), "wb");
            if (f == NULL) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            const void *buff;
            size_t size_;
            la_int64_t offset;
            while ((r = archive_read_data_block(a, &buff, &size_, &offset)) == ARCHIVE_OK) {
                if (fwrite(buff, 1, size_, f) != size_) {
                    fclose(f);
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }

            fclose(f);
            if (r != ARCHIVE_EOF) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }

        if (chown(full_path.c_str(), uid, gid) != 0 ||
            chmod(full_path.c_str(), mode) != 0 ||
            utime(full_path.c_str(), &(struct timeval){mtime, mtime}) != 0) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```