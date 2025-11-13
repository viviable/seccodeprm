```cpp
#include <archive.h>
#include <archive_entry.h>
#include <iostream>
#include <fstream>
#include <string>

bool extract_archive(struct archive *src, const char *dest_path) {
    struct archive *out;
    struct archive_entry *entry;
    int flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM;
    int r;
    char full_path[4096];

    // Initialize the archive writer
    out = archive_write_disk_new();
    if (!out) {
        return false;
    }
    archive_write_disk_set_options(out, flags);

    // Read and extract each entry
    while ((r = archive_read_next_header(src, &entry)) == ARCHIVE_OK) {
        // Create full path
        snprintf(full_path, sizeof(full_path), "%s/%s",
                dest_path, archive_entry_pathname(entry));
        archive_entry_set_pathname(entry, full_path);

        // Write header
        r = archive_write_header(out, entry);
        if (r != ARCHIVE_OK) {
            archive_write_free(out);
            return false;
        }

        // Copy data
        const void *buff;
        size_t size;
        la_int64_t offset;
        while ((r = archive_read_data_block(src, &buff, &size, &offset)) == ARCHIVE_OK) {
            if (archive_write_data_block(out, buff, size, offset) != ARCHIVE_OK) {
                archive_write_free(out);
                return false;
            }
        }
        if (r != ARCHIVE_EOF) {
            archive_write_free(out);
            return false;
        }
    }

    archive_write_free(out);
    return r == ARCHIVE_EOF ? true : false;
}

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Failed to open archive: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    if ((r = archive_read_next_header(a, &entry)) != ARCHIVE_OK) {
        std::cerr << "Failed to read first header from archive: " << archive_error_string(a) << std::endl;
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    while (true) {
        if ((r = extract_archive(a, dest_path.c_str())) != true) {
            std::cerr << "Extraction failed" << std::endl;
            break;
        }

        if ((r = archive_read_next_header(a, &entry)) != ARCHIVE_OK) {
            if (r == ARCHIVE_EOF) {
                break;
            } else {
                std::cerr << "Failed to read next header from archive: " << archive_error_string(a) << std::endl;
                break;
            }
        }
    }

    archive_read_close(a);
    archive_read_free(a);
    return r == ARCHIVE_EOF ? true : false;
}
```