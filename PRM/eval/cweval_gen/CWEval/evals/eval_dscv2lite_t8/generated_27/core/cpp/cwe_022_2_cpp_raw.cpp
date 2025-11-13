```cpp
#include <archive.h>
#include <archive_entry.h>
#include <iostream>
#include <fstream>
#include <cstring>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        archive_read_free(a);
        std::cerr << "Error opening archive: " << archive_error_string(a) << std::endl;
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        mode_t mode = archive_entry_mode(entry);
        time_t mtime = archive_entry_mtime(entry);

        // Create directories
        if (S_ISDIR(mode)) {
            if (mkdir(pathname, mode) != 0 && errno != EEXIST) {
                std::cerr << "Error creating directory: " << pathname << ", " << strerror(errno) << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        } else {
            // Create parent directories
            std::string dir_path = pathname;
            size_t pos = dir_path.find_last_of('/');
            if (pos != std::string::npos) {
                dir_path.resize(pos);
                if (mkdir(dir_path.c_str(), 0755) != 0 && errno != EEXIST) {
                    std::cerr << "Error creating directory: " << dir_path << ", " << strerror(errno) << std::endl;
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }

            // Extract file
            std::ofstream outfile(pathname, std::ios::binary);
            if (!outfile.is_open()) {
                std::cerr << "Error opening file for writing: " << pathname << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            const void *buff;
            size_t size;
            off_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                outfile.write(static_cast<const char*>(buff), size);
            }
            if (r != ARCHIVE_EOF) {
                std::cerr << "Error reading archive data" << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            outfile.close();
        }

        // Set file permissions and timestamps
        if (chmod(pathname, mode) != 0) {
            std::cerr << "Error setting file permissions: " << pathname << ", " << strerror(errno) << std::endl;
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
        if (utime(pathname, &mtime) != 0) {
            std::cerr << "Error setting file timestamp: " << pathname << ", " << strerror(errno) << std::endl;
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF;
}
```