```cpp
#include <archive.h>
#include <archive_entry.h>
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a = archive_read_new();
    if (a == nullptr) {
        return false;
    }
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);

    if (archive_read_open_filename(a, tar_path.c_str(), 10240) != ARCHIVE_OK) {
        archive_read_free(a);
        return false;
    }

    bool result = extract_archive(a, dest_path.c_str());

    archive_read_close(a);
    archive_read_free(a);

    return result;
}
```