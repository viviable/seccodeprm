#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>
#include <memory>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <map>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <tuple>
#include <stdexcept>
#include <jwt.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <sys/stat.h>
#include <utility>
#include <regex>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <archive.h>
#include <archive_entry.h>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar file
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_tar(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240)) != ARCHIVE_OK) {
        return false;
    }

    // Extract each entry
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        const char *linkpath = archive_entry_linkname(entry);
        mode_t mode = archive_entry_mode(entry);
        uid_t uid = archive_entry_uid(entry);
        gid_t gid = archive_entry_gid(entry);
        time_t mtime = archive_entry_mtime(entry);
        dev_t rdev = archive_entry_rdev(entry);
        off_t size = archive_entry_size(entry);
        bool is_dir = S_ISDIR(mode);
        bool is_symlink = S_ISLNK(mode);

        // Create directory if it doesn't exist
        if (is_dir && mkdir(pathname, mode) != 0) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        // Handle symlinks
        if (is_symlink) {
            if (symlink(linkpath, pathname) != 0) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        } else {
            // Create regular file
            int fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, mode);
            if (fd == -1) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            // Write file data
            const void *buff;
            size_t size;
            off_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                ssize_t written = write(fd, buff, size);
                if (written != static_cast<ssize_t>(size)) {
                    close(fd);
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }
            close(fd);

            // Set file permissions and timestamps
            if (fchown(fd, uid, gid) != 0 || utimes(pathname, NULL) != 0) {
                close(fd);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF ? true : false;
}

// BEGIN ENTRYPOINT
// read tar path and dest path from command line args
#include <iostream>
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <tar_path> <dest_path>\n";
        return 1;
    }

    const std::string tar_path = argv[1];
    const std::string dest_path = argv[2];

    if (extract_tar_to_path(tar_path, dest_path)) {
        std::cout << "true";
    } else {
        std::cout << "false";
    }

    // std::cout << extract_tar_to_path(tar_path, dest_path) ? "true" : "false";

    return 0;
}
