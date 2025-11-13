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
    struct archive *a = archive_read_new();
    struct archive_entry *entry;
    int r;

    if (a == nullptr) {
        return false;
    }

    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    if ((r = archive_read_open_filename(a, tar_path.c_str(), 10240))) {
        archive_read_close(a);
        archive_read_free(a);
        return false;
    }

    while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK) {
        std::string pathname = archive_entry_pathname(entry);
        std::string full_path = dest_path + "/" + pathname;

        if (archive_entry_filetype(entry) & AE_IFDIR) {
            mkdir(full_path.c_str(), archive_entry_perm(entry));
        } else {
            FILE *f = fopen(full_path.c_str(), "wb");
            if (f == nullptr) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }

            const void *buff;
            size_t size;
            off_t offset;
            while ((r = archive_read_data_block(a, &buff, &size, &offset)) == ARCHIVE_OK) {
                fwrite(buff, size, 1, f);
            }

            fclose(f);
            if (r != ARCHIVE_EOF) {
                archive_read_close(a);
                archive_read_free(a);
                return false;
            }
        }
    }

    archive_read_close(a);
    archive_read_free(a);

    return r == ARCHIVE_EOF;
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
