```cpp
#include <archive.h>
#include <archive_entry.h>
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a = archive_read_new();
    struct archive_entry *entry;
    int r;

    // Set up the archive reader
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);

    // Open the tar file
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240)) != ARCHIVE_OK) {
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        // Get the path name
        const char *pathname = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + pathname;

        // Handle directories
        if (archive_entry_filetype(entry) == AE_IFDIR) {
            if (mkdir(full_path.c_str(), 0755) != 0 && errno != EEXIST) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        } else {
            // Handle files
            FILE *f = fopen(full_path.c_str(), "wb");
            if (!f) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            const void *buff;
            size_t size;
            la_int64_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                if (fwrite(buff, size, 1, f) != 1) {
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
    }

    archive_read_close(a);
    archive_read_free(a);
    return r == ARCHIVE_EOF ? true : false;
}
```