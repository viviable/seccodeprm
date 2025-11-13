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
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if ((r = archive_read_open_filename(a, tar_path, 10240))) {
        fprintf(stderr, "Failed to open archive %s\n", tar_path);
        archive_read_free(a);
        return false;
    }

    // Create destination directory if it doesn't exist
    struct stat st;
    if (stat(dest_path, &st) != 0 || !S_ISDIR(st.st_mode)) {
        if (mkdir(dest_path, 0755) != 0) {
            fprintf(stderr, "Failed to create directory %s\n", dest_path);
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
    }

    // Extract the contents
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        char entry_path[PATH_MAX];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", dest_path, archive_entry_pathname(entry));

        // Create directories as needed
        char *dir = dirname(strdup(entry_path));
        if (stat(dir, &st) != 0 || !S_ISDIR(st.st_mode)) {
            if (mkdir(dir, 0755) != 0) {
                fprintf(stderr, "Failed to create directory %s\n", dir);
                free(dir);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
        free(dir);

        // Extract the file
        FILE *f = fopen(entry_path, "wb");
        if (!f) {
            fprintf(stderr, "Failed to open file %s for writing\n", entry_path);
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
                fprintf(stderr, "Failed to write to file %s\n", entry_path);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
        fclose(f);
        if (r != ARCHIVE_EOF) {
            fprintf(stderr, "Error reading data from archive\n");
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
    }

    // Clean up
    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}
```