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
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Error opening archive: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    // Extract the contents
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        if (archive_entry_filetype(entry) == AE_IFREG) {
            FILE *f = fopen(full_path.c_str(), "wb");
            if (!f) {
                std::cerr << "Error creating file: " << full_path << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            const void *buff;
            size_t size;
            la_int64_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                if (fwrite(buff, size, 1, f) != 1) {
                    std::cerr << "Error writing to file: " << full_path << std::endl;
                    fclose(f);
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }
            fclose(f);
            if (r != ARCHIVE_EOF) {
                std::cerr << "Error reading from archive: " << archive_error_string(a) << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        } else if (archive_entry_filetype(entry) == AE_IFDIR) {
            mkdir(full_path.c_str(), archive_entry_mode(entry) & 0777);
        } else {
            std::cerr << "Unsupported file type: " << archive_entry_filetype(entry) << std::endl;
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