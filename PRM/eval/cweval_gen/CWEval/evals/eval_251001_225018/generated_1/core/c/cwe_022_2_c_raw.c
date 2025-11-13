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

    // Open the tar archive
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);

    if (archive_read_open_filename(a, tar_path, 10240) != ARCHIVE_OK) {
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        char full_path[PATH_MAX];
        struct stat st;

        // Create full path
        snprintf(full_path, sizeof(full_path), "%s/%s", dest_path, archive_entry_pathname(entry));

        // Ensure parent directory exists
        char *dir = strdup(full_path);
        char *parent_dir = dirname(dir);
        if (mkdir(parent_dir, 0755) != 0 && errno != EEXIST) {
            free(dir);
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
        free(dir);

        // Set permissions and times
        memset(&st, 0, sizeof(st));
        st.st_mode = archive_entry_perm(entry);
        st.st_uid = archive_entry_uid(entry);
        st.st_gid = archive_entry_gid(entry);
        st.st_mtime = archive_entry_mtime(entry);
        if (utime(full_path, &st) != 0) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        // Write the file data
        FILE *f = fopen(full_path, "wb");
        if (!f) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        while ((r = archive_read_data(a, f, 8192)) > 0) {
            // Continue reading until EOF
        }
        fclose(f);

        if (r != ARCHIVE_EOF) {
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