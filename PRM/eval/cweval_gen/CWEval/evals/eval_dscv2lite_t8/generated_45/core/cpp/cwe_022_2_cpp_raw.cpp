```cpp
#include <archive.h>
#include <archive_entry.h>
#include <iostream>
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar archive
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Error opening archive: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    // Extract the contents
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        mode_t mode = archive_entry_mode(entry);
        uid_t uid = archive_entry_uid(entry);
        gid_t gid = archive_entry_gid(entry);
        time_t mtime = archive_entry_mtime(entry);

        // Ensure directory structure exists
        size_t pos = 0;
        while ((pos = full_path.find('/', pos)) != std::string::npos) {
            std::string dir = full_path.substr(0, pos);
            if (dir.empty() || dir == ".") continue;
            if (mkdir(dir.c_str(), 0755) == -1 && errno != EEXIST) {
                std::cerr << "Error creating directory " << dir << ": " << strerror(errno) << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            ++pos;
        }

        // Handle regular files
        if (S_ISREG(mode)) {
            FILE *f = fopen(full_path.c_str(), "wb");
            if (!f) {
                std::cerr << "Error opening file " << full_path << " for writing: " << strerror(errno) << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            const void *buff;
            size_t size;
            la_int64_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                if (fwrite(buff, size, 1, f) != 1) {
                    std::cerr << "Error writing to file " << full_path << ": " << strerror(errno) << std::endl;
                    fclose(f);
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }
            fclose(f);
        } else if (S_ISDIR(mode)) {
            // Directories are already created above
            continue;
        } else {
            std::cerr << "Unsupported file type encountered: " << full_path << std::endl;
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        // Set file permissions and timestamps
        if (chmod(full_path.c_str(), mode) == -1) {
            std::cerr << "Error setting permissions on " << full_path << ": " << strerror(errno) << std::endl;
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
        if (chown(full_path.c_str(), uid, gid) == -1) {
            std::cerr << "Error changing ownership of " << full_path << ": " << strerror(errno) << std::endl;
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
        if (utime(full_path.c_str(), &mtime) == -1) {
            std::cerr << "Error setting timestamp on " << full_path << ": " << strerror(errno) << std::endl;
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