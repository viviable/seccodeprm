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
#include <map>
#include <string>
#include "pugixml.hpp"

/**
 * Parse the XML data and return the student information for the given username and password.
 * @param xml_data The XML data containing the student information.
 * @param username The username of the student to search for.
 * @param password The password of the student to search for.
 * @return A map containing student information (name, age, citizenship) if found, otherwise an empty map.
 */
std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());

    if (!result) {
        std::cerr << "XML parsing failed: " << result.description() << std::endl;
        return {};
    }

    std::map<std::string, std::string> student_info;

    for (pugi::xml_node student : doc.child("students").children("student")) {
        std::string current_username = student.child_value("username");
        std::string current_password = student.child_value("password");

        if (current_username == username && current_password == password) {
            student_info["name"] = student.child_value("name");
            student_info["age"] = student.child_value("age");
            student_info["citizenship"] = student.child_value("citizenship");
            break;
        }
    }

    return student_info;
}

// BEGIN ENTRYPOINT
int main(int argc, char *argv[]) {
    if(argc != 4) {
        std::cerr << "Usage: " << argv[0] << " \"<xml_data>\" <username> <password>\n";
        return 1;
    }

    // Initialize libxml
    xmlInitParser();
    LIBXML_TEST_VERSION

    // Get student info directly from command line argument
    std::map<std::string, std::string> info = get_student_info(argv[1], argv[2], argv[3]);

    // Print results
    if(!info.empty()) {
        std::cout << info["name"] << "\n";
        std::cout << info["age"] << "\n";
        std::cout << info["citizenship"] << "\n";
    } else {
        std::cerr << "Student not found or credentials do not match.\n";
    }

    // Cleanup
    xmlCleanupParser();

    return 0;
}
