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
#include <fstream>
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
    archive_read_support_format_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Failed to open archive" << std::endl;
        return false;
    }

    // Extract the contents
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string full_path = dest_path + "/" + archive_entry_pathname(entry);
        mode_t mode = archive_entry_perm(entry);

        // Create directories
        if (S_ISDIR(mode)) {
            if (mkdir(full_path.c_str(), mode) != 0 && errno != EEXIST) {
                std::cerr << "Failed to create directory: " << full_path << std::endl;
                archive_read_close(a);
                archive_read_finish(a);
                return false;
            }
        } else {
            // Create parent directories
            size_t pos = full_path.find_last_of('/');
            if (pos != std::string::npos) {
                std::string parent_dir = full_path.substr(0, pos);
                if (mkdir(parent_dir.c_str(), 0755) != 0 && errno != EEXIST) {
                    std::cerr << "Failed to create directory: " << parent_dir << std::endl;
                    archive_read_close(a);
                    archive_read_finish(a);
                    return false;
                }
            }

            // Open the output file
            FILE *f = fopen(full_path.c_str(), "wb");
            if (!f) {
                std::cerr << "Failed to open file for writing: " << full_path << std::endl;
                archive_read_close(a);
                archive_read_finish(a);
                return false;
            }

            // Write the file data
            const void *buff;
            size_t size;
            la_int64_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                if (fwrite(buff, size, 1, f) != 1) {
                    std::cerr << "Failed to write to file: " << full_path << std::endl;
                    fclose(f);
                    archive_read_close(a);
                    archive_read_finish(a);
                    return false;
                }
            }
            fclose(f);

            // Set file permissions
            chmod(full_path.c_str(), mode);
        }
    }

    archive_read_close(a);
    archive_read_finish(a);
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
