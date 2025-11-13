```cpp
#include <archive.h>
#include <archive_entry.h>
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int ret;

    // Open the tar archive
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);
    if ((ret = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        return false;
    }

    // Extract each entry from the archive
    while ((ret = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        mode_t mode = archive_entry_mode(entry);

        // Create directories if necessary
        if (S_ISDIR(mode)) {
            mkdir(full_path.c_str(), mode);
            continue;
        }

        // Open the output file
        FILE *f = fopen(full_path.c_str(), "wb");
        if (!f) {
            return false;
        }

        // Copy data from the archive to the output file
        const void *buffer;
        size_t size;
        off_t offset;
        while ((ret = archive_read_data_block(a, &buffer, &size, &offset)) == ARCHIVE_OK) {
            fwrite(buffer, size, 1, f);
        }
        fclose(f);

        // Handle errors during reading or writing
        if (ret != ARCHIVE_EOF && ret != ARCHIVE_RETRY) {
            return false;
        }
    }

    // Clean up
    archive_read_close(a);
    archive_read_free(a);

    return ret == ARCHIVE_EOF;
}
```