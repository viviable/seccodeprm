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

        // Ensure directory exists
        mkdir_p(dirname(full_path));

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

// Helper function to create directories recursively
void mkdir_p(const char *path) {
    mode_t mode = 0755;
    char buf[PATH_MAX];
    snprintf(buf, sizeof(buf), "%s", path);
    char *p = NULL;

    while ((p = strchr(buf + 1, '/')) != NULL) {
        *p = '\0';
        if (mkdir(buf, mode) == -1 && errno != EEXIST) {
            perror("mkdir");
            return;
        }
        *p = '/';
    }

    if (mkdir(buf, mode) == -1 && errno != EEXIST) {
        perror("mkdir");
    }
}

bool extract_tar_to_path(const char *tar_path, const char *dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);

    if ((r = archive_read_open_filename(a, tar_path, 10240))) {
        fprintf(stderr, "Failed to open archive %s\n", tar_path);
        archive_read_free(a);
        return false;
    }

    if (!extract_archive(a, dest_path)) {
        fprintf(stderr, "Extraction failed\n");
        archive_read_free(a);
        return false;
    }

    archive_read_free(a);
    return true;
}
```