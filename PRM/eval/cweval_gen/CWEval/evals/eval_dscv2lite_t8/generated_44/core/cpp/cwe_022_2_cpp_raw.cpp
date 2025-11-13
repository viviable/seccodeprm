```cpp
#include <archive.h>
#include <archive_entry.h>
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
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    // Initialize the archive reader
    if ((r = archive_read_extract_header(a, &entry))) {
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        // Get the full path for the extracted file
        const char *pathname = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + pathname;

        // Ensure the directory structure exists
        size_t pos;
        while ((pos = full_path.find('/', pos + 1)) != std::string::npos) {
            std::string dir = full_path.substr(0, pos);
            mkdir(dir.c_str(), 0755);
        }

        // Write the file content
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
            if (fwrite(buff, 1, size, f) != size) {
                fclose(f);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
        fclose(f);

        // Set file permissions
        chmod(full_path.c_str(), archive_entry_perm(entry));
    }

    // Close the archive
    archive_read_close(a);
    archive_read_free(a);
    return true;
}
```