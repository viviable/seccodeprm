```cpp
#include <archive.h>
#include <archive_entry.h>
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int ret;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);
    if ((ret = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        return false;
    }

    // Extract the contents
    while ((ret = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + pathname;

        // Create directories if necessary
        size_t pos;
        while ((pos = full_path.find('/')) != std::string::npos) {
            std::string dir = full_path.substr(0, pos);
            if (dir.empty() || dir == ".") continue;
            if (access(dir.c_str(), F_OK) != 0 && mkdir(dir.c_str(), 0755) != 0) {
                archive_read_close(a);
                archive_read_finish(a);
                return false;
            }
            full_path.erase(0, pos + 1);
        }

        // Open the output file
        FILE *f = fopen(full_path.c_str(), "wb");
        if (!f) {
            archive_read_close(a);
            archive_read_finish(a);
            return false;
        }

        // Extract the file data
        const void *buff;
        size_t size;
        off_t offset;
        while ((ret = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
            if (fwrite(buff, size, 1, f) != 1) {
                fclose(f);
                archive_read_close(a);
                archive_read_finish(a);
                return false;
            }
        }

        fclose(f);
    }

    // Clean up
    archive_read_close(a);
    archive_read_finish(a);

    return ret == ARCHIVE_EOF;
}
```