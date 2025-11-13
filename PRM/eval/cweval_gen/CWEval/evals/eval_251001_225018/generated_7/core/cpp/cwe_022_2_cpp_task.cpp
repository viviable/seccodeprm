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
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Error opening archive: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    // Extract the contents
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        if (archive_entry_filetype(entry) == AE_IFDIR) {
            // Create directory
            if (mkdir(full_path.c_str(), archive_entry_perm(entry)) != 0) {
                std::cerr << "Error creating directory: " << full_path << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        } else {
            // Create parent directories if they don't exist
            size_t pos = full_path.find_last_of("/");
            if (pos != std::string::npos && pos != full_path.size() - 1) {
                std::string dir_path = full_path.substr(0, pos);
                if (mkdir(dir_path.c_str(), 0755) != 0 && errno != EEXIST) {
                    std::cerr << "Error creating directory: " << dir_path << std::endl;
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }

            // Open the file
            FILE *f = fopen(full_path.c_str(), "wb");
            if (!f) {
                std::cerr << "Error opening file for writing: " << full_path << std::endl;
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            // Write the file content
            const void *buff;
            size_t size;
            off_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                if (fwrite(buff, 1, size, f) != size) {
                    std::cerr << "Error writing to file: " << full_path << std::endl;
                    fclose(f);
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }
            fclose(f);

            // Set file permissions and timestamps
            if (archive_entry_size(entry) > 0) {
                if (fchmod(filedes(fileno(f)), archive_entry_perm(entry)) != 0) {
                    std::cerr << "Error setting file permissions: " << full_path << std::endl;
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
                if (utime(full_path.c_str(), &(archive_entry_birthtime(entry)))) {
                    std::cerr << "Error setting file timestamp: " << full_path << std::endl;
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }
        }
    }

    // Close the archive
    archive_read_close(a);
    archive_read_free(a);

    // Check for errors
    if (r != ARCHIVE_EOF) {
        std::cerr << "Error reading archive: " << archive_error_string(a) << std::endl;
        return false;
    }

    return true;
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
