```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <archive.h>
#include <archive_entry.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>

#define PATH_MAX 512

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

bool extract_tar_to_path(const char *tar_path, const char *dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    if ((r = archive_read_open_filename(a, tar_path, 10240))) {
        fprintf(stderr, "Error opening archive %s\n", tar_path);
        archive_read_free(a);
        return false;
    }

    // Ensure the destination directory exists
    struct stat st;
    if (stat(dest_path, &st) != 0 || !S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Destination path is not a valid directory: %s\n", dest_path);
        archive_read_free(a);
        return false;
    }

    // Extract the contents
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        const char *basename = basename((char *)pathname);
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", dest_path, basename);

        // Create directories as needed
        char dir_path[PATH_MAX];
        strncpy(dir_path, full_path, sizeof(dir_path));
        dir_path[sizeof(dir_path) - 1] = '\0';
        char *p = strrchr(dir_path, '/');
        if (p) {
            *p = '\0';
            if (mkdir(dir_path, 0755) != 0 && errno != EEXIST) {
                fprintf(stderr, "Failed to create directory: %s\n", dir_path);
                archive_read_free(a);
                return false;
            }
        }

        // Extract the file
        FILE *f = fopen(full_path, "wb");
        if (!f) {
            fprintf(stderr, "Failed to open file for writing: %s\n", full_path);
            archive_read_free(a);
            return false;
        }
        while ((r = archive_read_data_block(a, NULL, NULL, NULL)) == ARCHIVE_OK) {
            ssize_t bytes_written = fwrite(archive_read_data(a), 1, archive_read_data_skip(a), f);
            if (bytes_written != archive_read_data_skip(a)) {
                fclose(f);
                fprintf(stderr, "Failed to write data to file: %s\n", full_path);
                archive_read_free(a);
                return false;
            }
        }
        fclose(f);
        if (r != ARCHIVE_EOF) {
            archive_read_free(a);
            return false;
        }
    }

    archive_read_close(a);
    archive_read_free(a);
    return r == ARCHIVE_EOF ? true : false;
}
```