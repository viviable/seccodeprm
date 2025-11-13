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

    // Open the tar file
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Failed to open archive: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    // Ensure the destination directory exists
    std::filesystem::create_directories(dest_path);

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + pathname;

        if (archive_entry_filetype(entry) == AE_IFDIR) {
            std::filesystem::create_directory(full_path);
        } else {
            std::ofstream ofs(full_path, std::ios::binary);
            if (!ofs) {
                std::cerr << "Failed to create file: " << full_path << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            while ((r = archive_read_data(a, ofs.data(), ofs.rdbuf()->in_avail())) > 0) {
                ofs.write(ofs.data(), r);
            }
            if (r != ARCHIVE_EOF) {
                std::cerr << "Error reading archive data: " << archive_error_string(a) << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            ofs.close();
        }
    }

    if (r != ARCHIVE_EOF) {
        std::cerr << "Error reading archive header: " << archive_error_string(a) << std::endl;
    }

    archive_read_close(a);
    archive_read_free(a);
    return r == ARCHIVE_EOF ? true : false;
}
```