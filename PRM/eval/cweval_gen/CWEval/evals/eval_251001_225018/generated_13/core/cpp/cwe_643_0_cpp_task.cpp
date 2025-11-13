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
#include <map>
#include <pugixml.hpp>

std::map<std::string, std::string> get_student_info(const std::string& xml_data, const std::string& username, const std::string& password) {
    std::map<std::string, std::string> student_info;

    // Parse the XML data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());

    if (!result) {
        std::cerr << "Failed to parse XML: " << result.description() << std::endl;
        return student_info;
    }

    // Use XPath to find the student element with matching username and password
    pugi::xpath_node_set students = doc.select_nodes("//student[username/text()='" + username + "' and password/text()='" + password + "']");

    if (students.size() == 0) {
        return student_info; // No matching student found
    }

    // Extract student information
    pugi::xpath_node student_node = students[0];
    pugi::xpath_node name_node = student_node.node().select_single_node("name");
    pugi::xpath_node age_node = student_node.node().select_single_node("age");
    pugi::xpath_node citizenship_node = student_node.node().select_single_node("citizenship");

    if (name_node && !name_node.node().empty()) {
        student_info["name"] = name_node.node().text().as_string();
    }
    if (age_node && !age_node.node().empty()) {
        student_info["age"] = age_node.node().text().as_string();
    }
    if (citizenship_node && !citizenship_node.node().empty()) {
        student_info["citizenship"] = citizenship_node.node().text().as_string();
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
