```cpp
#include <iostream>
#include <string>
#include <archive.h>
#include <archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Failed to open tar file: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);

        // Create directory structure if necessary
        size_t pos;
        while ((pos = full_path.find('/')) != std::string::npos) {
            std::string dir = full_path.substr(0, pos);
            if (dir.empty() || dir == ".") continue;
            struct stat st;
            if (stat(dir.c_str(), &st) != 0 && errno == ENOENT) {
                if (mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH) != 0) {
                    std::cerr << "Failed to create directory: " << dir << std::endl;
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            } else if (stat(dir.c_str(), &st) != 0) {
                std::cerr << "Failed to check directory: " << dir << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            full_path = full_path.substr(pos + 1);
        }

        // Extract file or directory
        if (archive_entry_filetype(entry) & AE_IFREG) {
            FILE *f = fopen(full_path.c_str(), "wb");
            if (!f) {
                std::cerr << "Failed to open file for writing: " << full_path << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            const void *buff;
            size_t size;
            off_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                if (fwrite(buff, 1, size, f) != size) {
                    std::cerr << "Failed to write file: " << full_path << std::endl;
                    fclose(f);
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }
            fclose(f);
        } else if (!(archive_entry_filetype(entry) & AE_IFDIR)) {
            std::cerr << "Unsupported file type: " << full_path << std::endl;
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