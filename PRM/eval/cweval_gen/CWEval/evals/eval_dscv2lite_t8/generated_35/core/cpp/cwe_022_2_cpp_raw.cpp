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
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Could not open archive: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string pathname = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + pathname;

        // Create directory structure if necessary
        size_t pos = 0;
        while ((pos = full_path.find('/', pos + 1)) != std::string::npos) {
            std::string dir = full_path.substr(0, pos);
            if (mkdir(dir.c_str(), 0755) == -1 && errno != EEXIST) {
                std::cerr << "Could not create directory: " << dir << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }

        // Extract file
        FILE *f = fopen(full_path.c_str(), "wb");
        if (!f) {
            std::cerr << "Could not create file: " << full_path << std::endl;
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        const void *buff;
        size_t size;
        off_t offset;
        while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
            if (fwrite(buff, 1, size, f) != size) {
                std::cerr << "Error writing to file: " << full_path << std::endl;
                fclose(f);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
        fclose(f);

        // Set file permissions
        if (archive_entry_perm(entry) & 0x8000) { // Regular file
            chmod(full_path.c_str(), archive_entry_perm(entry));
        } else if (archive_entry_perm(entry) & 0x4000) { // Directory
            // Directories were already created above
        }
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```