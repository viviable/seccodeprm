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
#include <archive.h>
#include <archive_entry.h>
#include <iostream>
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Open the tar archive
    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        std::cerr << "Error opening archive: " << archive_error_string(a) << std::endl;
        archive_read_free(a);
        return false;
    }

    // Extract each entry from the tar archive
    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + pathname;

        // Ensure directory structure exists
        size_t pos = 0;
        while ((pos = full_path.find('/', pos)) != std::string::npos) {
            std::string dir = full_path.substr(0, pos + 1);
            if (dir.empty() || dir == "/") continue;
            if (access(dir.c_str(), F_OK) != 0) {
                if (mkdir(dir.c_str(), 0755) != 0) {
                    std::cerr << "Error creating directory: " << dir << std::endl;
                    archive_read_close(a);
                    archive_read_free(a);
                    return false;
                }
            }
            pos++;
        }

        // Open the file for writing
        FILE *f = fopen(full_path.c_str(), "wb");
        if (!f) {
            std::cerr << "Error opening file for writing: " << full_path << std::endl;
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        // Copy data from the archive to the file
        const void *buff;
        size_t size;
        la_int64_t offset;
        while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
            if (fwrite(buff, size, 1, f) != 1) {
                std::cerr << "Error writing to file: " << full_path << std::endl;
                fclose(f);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
        fclose(f);

        // Check for errors reading from the archive
        if (r != ARCHIVE_EOF) {
            std::cerr << "Error reading archive: " << archive_error_string(a) << std::endl;
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
    }

    // Close the archive and free resources
    archive_read_close(a);
    archive_read_free(a);

    // Return success if all entries were extracted successfully
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
