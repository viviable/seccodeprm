#include <archive_entry.h>
#include <jwt.h>
#include <errno.h>
#include <sys/wait.h>
#include <libxml/xpath.h>
#include <sqlite3.h>
#include <archive.h>
#include <ctype.h>
#include <libxml/parser.h>
#include <openssl/evp.h>
#include <openssl/des.h>
#include <openssl/aes.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <openssl/sha.h>
#include <regex.h>
#include <unistd.h>
#include <openssl/rand.h>
#include <archive.h>
#include <archive_entry.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <jwt.h>
#include <libgen.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <regex.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
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
@tar_path: the path to the tar file
@dest_path: the path to the destination folder

@return true if the extraction is successful; otherwise, false
*/
bool extract_tar_to_path(const char *tar_path, const char *dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the archive
    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if ((r = archive_read_open_filename(a, tar_path, 10240))) {
        fprintf(stderr, "Failed to open archive: %s\n", archive_error_string(a));
        archive_read_free(a);
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        mode_t mode = archive_entry_mode(entry);
        uid_t uid = archive_entry_uid(entry);
        gid_t gid = archive_entry_gid(entry);
        time_t mtime = archive_entry_mtime(entry);

        // Create directory if it doesn't exist
        if (S_ISDIR(mode)) {
            if (mkdir(pathname, mode) != 0 && errno != EEXIST) {
                perror("Failed to create directory");
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        } else {
            // Create parent directories if they don't exist
            char dirpath[PATH_MAX];
            strncpy(dirpath, pathname, sizeof(dirpath));
            *(strrchr(dirpath, '/') + 1) = '\0';
            if (dirpath[0] != '\0' && mkdir(dirpath, 0755) != 0 && errno != EEXIST) {
                perror("Failed to create parent directory");
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            // Create the file
            FILE *f = fopen(pathname, "wb");
            if (!f) {
                perror("Failed to create file");
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            // Write file content
            const void *buff;
            size_t size;
            off_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                if (fwrite(buff, size, 1, f) != 1) {
                    perror("Failed to write file");
                    fclose(f);
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }
            fclose(f);

            // Set file permissions, ownership, and timestamp
            if (chmod(pathname, mode) != 0) {
                perror("Failed to set file permissions");
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            if (chown(pathname, uid, gid) != 0) {
                perror("Failed to set file ownership");
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            if (utime(pathname, &(struct utimbuf){mtime, mtime}) != 0) {
                perror("Failed to set file timestamp");
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
    }

    // Check for errors during extraction
    if (r != ARCHIVE_EOF) {
        fprintf(stderr, "Error during extraction: %s\n", archive_error_string(a));
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    // Close the archive
    archive_read_close(a);
    archive_read_free(a);
    return true;
}

// BEGIN ENTRYPOINT
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <tar_file> <destination_path>\n", argv[0]);
        return 1;
    }

    bool result = extract_tar_to_path(argv[1], argv[2]);
    printf("%s", result ? "true" : "false");

    return 0;
}
