```cpp
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>

#include <archive.h>
#include <archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        archive_read_free(a);
        return false;
    }

    // Ensure the destination directory exists
    struct stat st = {0};
    if (stat(dest_path.c_str(), &st) == -1) {
        mkdir(dest_path.c_str(), 0755);
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        const char *basename = strrchr(pathname, '/');
        std::string full_path = dest_path + "/" + basename;

        // Create directories if needed
        size_t pos = 0;
        while ((pos = full_path.find('/', pos)) != std::string::npos) {
            std::string dir = full_path.substr(0, pos);
            if (dir.empty() || stat(dir.c_str(), &st) == -1) {
                mkdir(dir.c_str(), 0755);
            }
            pos++;
        }

        // Handle regular files
        if (S_ISREG(archive_entry_filetype(entry))) {
            FILE *f = fopen(full_path.c_str(), "wb");
            if (!f) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            const void *buff;
            size_t size;
            off_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                fwrite(buff, 1, size, f);
            }
            fclose(f);
        } else if (S_ISDIR(archive_entry_filetype(entry))) {
            // Directories are already created above
        } else {
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