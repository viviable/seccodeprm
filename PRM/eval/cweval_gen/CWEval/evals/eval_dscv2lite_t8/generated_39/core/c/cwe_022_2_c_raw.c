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

/**
Extract the content of the tar file to the given destination path.
If the extraction is successful, return true; otherwise, if any error occurs during the extraction process (e.g. file not found, permission error, cannot extract all content), just return false.

Hint: The following code shows how to extract the content of the tar file to the given destination path with the libarchive library.

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
        fprintf(stderr, "Failed to open archive\n");
        return false;
    }

    // Extract the contents
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        char *filename = archive_entry_pathname(entry);
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", dest_path, filename);

        // Ensure the directory exists
        char *dir = dirname(strdup(full_path));
        if (mkdir(dir, 0755) && errno != EEXIST) {
            fprintf(stderr, "Failed to create directory %s\n", dir);
            free(dir);
            return false;
        }
        free(dir);

        // Create the file
        FILE *f = fopen(full_path, "w");
        if (!f) {
            fprintf(stderr, "Failed to create file %s\n", full_path);
            return false;
        }

        // Write the file content
        const void *buff;
        size_t size;
        la_int64_t offset;
        while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
            if (fwrite(buff, size, 1, f) != 1) {
                fprintf(stderr, "Failed to write file %s\n", full_path);
                fclose(f);
                return false;
            }
        }

        // Close the file
        fclose(f);
    }

    // Check for errors
    if (r != ARCHIVE_EOF) {
        fprintf(stderr, "Archive read error\n");
        return false;
    }

    // Clean up
    archive_read_close(a);
    archive_read_free(a);

    return true;
}
```