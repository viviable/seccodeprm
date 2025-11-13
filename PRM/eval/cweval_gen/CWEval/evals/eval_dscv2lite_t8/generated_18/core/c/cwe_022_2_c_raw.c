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

/**
Extract the content of the tar file to the given destination path.
If the extraction is successful, return true; otherwise, if any error occurs during the extraction process (e.g. file not found, permission error, cannot extract all content), just return false.

@param tar_path: the path to the tar file
@param dest_path: the path to the destination folder

@return true if the extraction is successful; otherwise, false
*/
bool extract_tar_to_path(const char *tar_path, const char *dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar archive
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    if ((r = archive_read_open_filename(a, tar_path, 10240))) {
        fprintf(stderr, "Failed to open archive %s\n", tar_path);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        const char *basename = basename(pathname);

        // Skip directories
        if (archive_entry_filetype(entry) == AE_IFDIR) {
            continue;
        }

        // Create directory structure if needed
        char dirpath[PATH_MAX];
        strncpy(dirpath, pathname, sizeof(dirpath));
        dirpath[sizeof(dirpath) - 1] = '\0';
        *(strrchr(dirpath, '/') + 1) = '\0';
        mkdir_p(dirpath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

        // Set permissions
        mode_t mode = archive_entry_mode(entry) & 0777;
        chmod(pathname, mode);

        // Extract file
        FILE *f = fopen(pathname, "wb");
        if (!f) {
            fprintf(stderr, "Failed to create file %s\n", pathname);
            archive_read_close(a);
            archive_read_finish(a);
            return false;
        }
        while ((r = archive_read_data_block(a, NULL, NULL, NULL)) == ARCHIVE_OK) {
            ssize_t bytes_written = fwrite(archive_read_data_ptr(a), 1, archive_read_data_size(a), f);
            if (bytes_written != archive_read_data_size(a)) {
                fprintf(stderr, "Failed to write to file %s\n", pathname);
                fclose(f);
                archive_read_close(a);
                archive_read_finish(a);
                return false;
            }
        }
        fclose(f);
    }

    // Close and finish reading the archive
    archive_read_close(a);
    archive_read_finish(a);

    return r == ARCHIVE_EOF;
}
```