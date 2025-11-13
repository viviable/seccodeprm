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
#include <string>

bool extract_tar_to_path(const std::string& tar_path, const std::string& dest_path) {
    struct archive *a = archive_read_new();
    struct archive_entry *entry;
    int ret;

    if (a == NULL) {
        return false;
    }

    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    if ((ret = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    while ((ret = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        const char *pathname = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + pathname;

        // Create directories if necessary
        size_t pos = full_path.find_first_of("/\\", 1);
        while (pos != std::string::npos) {
            std::string dir = full_path.substr(0, pos);
            if (mkdir(dir.c_str(), 0755) != 0 && errno != EEXIST) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
            pos = full_path.find_first_of("/\\", pos + 1);
        }

        FILE *f = fopen(full_path.c_str(), "wb");
        if (f == NULL) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }

        const void *buff;
        size_t size;
        la_int64_t offset;
        while ((ret = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
            if (fwrite(buff, size, 1, f) != 1) {
                fclose(f);
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
        fclose(f);

        if (ret != ARCHIVE_EOF) {
            archive_read_close(a);
            archive_read_free(a);
            return false;
        }
    }

    archive_read_close(a);
    archive_read_free(a);
    return ret == ARCHIVE_EOF;
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
